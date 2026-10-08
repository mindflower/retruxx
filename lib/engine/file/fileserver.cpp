#include <memory>
#include <stdexcept>
#include <cstring>
#include <io.h>
#include <core/ini.h>
#include <file/fileenum.h>
#include <file/filereader.h>
#include <file/fileserver.h>
#include <file/memoryfile.h>
#include <file/package.h>
#include <file/rawfile.h>
#include "retruxx/common.h"

namespace m3d
{
    namespace fs
    {
        namespace
        {
            // A name like "c:/..." or "c:\..." is a full path and bypasses the virtual file system.
            bool IsAbsolutePath(char const* filename)
            {
                return filename[1] == ':' && (filename[2] == '/' || filename[2] == '\\');
            }
        }  // namespace

        FileServer::FileServer()
        {
            // RVA 0x664EB0
        }

        FileServer::~FileServer()
        {
            // RVA 0x665030
            Shutdown();
        }

        bool FileServer::FileExists(char const* filename)
        {
            // RVA 0x6632D0 - a loose file known to the server, or a file in any package.
            if (IsAbsolutePath(filename))
            {
                CStr fileName(filename);
                UnifyFileName(fileName);
                return ::GetFileAttributesA(fileName.c_str()) != INVALID_FILE_ATTRIBUTES;
            }

            CStr fullFileName;
            DecryptFileName(filename, fullFileName);
            if (m_Files.find(fullFileName) != m_Files.end())
            {
                return true;
            }
            for (auto& package : m_Packages)
            {
                if (package->HaveFile(fullFileName.c_str()))
                {
                    return true;
                }
            }
            return false;
        }

        FileStream* FileServer::CreateFileStream()
        {
            // RVA 0x662470
            return new FileReader;
        }

        int FileServer::RemoveFile(char const* FileName)
        {
            // RVA 0x663C60
            CStr name(FileName);
            UnifyFileName0(name);
            FilesStorageType::iterator const it = m_Files.find(name);
            if (it != m_Files.end())
            {
                m_Files.erase(it);
            }
            return 0;
        }

        void FileServer::EnableMapping(bool State)
        {
            // RVA 0x661BC0
            m_EnableMapping = State;
        }

        int FileServer::Reinitialize(char const* dataSourcesFile)
        {
            // RVA 0x665400
            Shutdown();
            return Initialize(dataSourcesFile);
        }

        int FileServer::Shutdown()
        {
            // RVA 0x664F10
            m_Packages.clear();
            m_Files.clear();
            m_Initialized = false;
            return 1;
        }

        CriticalSection& FileServer::GetCriticalSecton()
        {
            // RVA 0x7574B0
            return m_cs;
        }

        int FileServer::OpenFileStream(FileReader* reader, char const* filename, IStream::OpenFlags flags)
        {
            // RVA 0x663740 - a full path is opened directly. Otherwise a file is read from the loose files first and
            // then from the packages; a file written is added to the loose files.
            if (IsAbsolutePath(filename))
            {
                CStr fileName(filename);
                UnifyFileName(fileName);
                delete reader->InternalObject;
                reader->InternalObject = new RawFile(fileName.c_str(), flags, m_EnableMapping);
                return reader->InternalObject->IsOpen();
            }

            CStr fullFileName;
            DecryptFileName(filename, fullFileName);
            if (flags == IStream::OPEN_READ)
            {
                if (m_Files.find(fullFileName) != m_Files.end())
                {
                    // NOTE: reported as opened even when the file cannot be.
                    delete reader->InternalObject;
                    reader->InternalObject = new RawFile(fullFileName.c_str(), IStream::OPEN_READ, m_EnableMapping);
                    return 1;
                }
                for (auto& package : m_Packages)
                {
                    if (package->HaveFile(fullFileName.c_str()))
                    {
                        delete reader->InternalObject;
                        reader->InternalObject = new MemoryFile(fullFileName.c_str(), package.get());
                        return 1;
                    }
                }
                // NOTE: an unknown file is not looked for on disk, and the reader keeps whatever it had open.
                return 0;
            }

            if (m_Files.find(fullFileName) == m_Files.end())
            {
                m_Files.insert(fullFileName);
            }
            delete reader->InternalObject;
            reader->InternalObject = new RawFile(fullFileName.c_str(), flags, m_EnableMapping);
            return reader->InternalObject->IsOpen() ? 1 : 0;
        }

        int FileServer::AddPackage(char const* PackageName)
        {
            // RVA 0x665450
            CStr name(PackageName);
            UnifyFileName(name);
            if (::GetFileAttributesA(name.c_str()) == INVALID_FILE_ATTRIBUTES)
            {
                return ::GetLastError();
            }
            return InternalAddPackage(name);
        }

        int FileServer::AddFile(char const* FileName)
        {
            // RVA 0x663690
            CStr fullFileName;
            DecryptFileName(FileName, fullFileName);
            UnifyFileName(fullFileName);
            if (GetFileAttributesA(fullFileName.c_str()) == -1)
            {
                return 2;
            }
            UnifyFileName0(fullFileName);
            m_Files.insert(fullFileName);
            return 0;
        }

        int FileServer::RemoveFolder(char const* FolderName)
        {
            // RVA 0x663A50 - forgets the loose files under the folder: the run of names starting with its path.
            CStr folder(FolderName);
            UnifyFileName0(folder);
            if (folder.c_str()[strlen(folder.c_str()) - 1] != '/')
            {
                folder += CStr("/");
            }
            size_t const length = strlen(folder.c_str());
            FilesStorageType::iterator it = m_Files.begin();
            while (it != m_Files.end() && strncmp(it->c_str(), folder.c_str(), length) != 0)
            {
                ++it;
            }
            while (it != m_Files.end() && strncmp(it->c_str(), folder.c_str(), length) == 0)
            {
                it = m_Files.erase(it);
            }
            return 0;
        }

        int FileServer::Initialize(char const* dataSource)
        {
            // RVA 0x665160 - the data sources file lists, one per line, the folders, loose files and .GDP packages
            // to serve. An optional first line of 0 or 1 turns file mapping off or on; a " M" after an entry maps
            // just that one.
            if (m_Initialized)
            {
                return -1;
            }

            auto file = std::make_unique<RawFile>(dataSource, IStream::OPEN_READ, m_EnableMapping);
            if (!file->IsOpen())
            {
                return file->Error();
            }
            unsigned int const size = file->GetSize();
            retruxx::vector<char> buffer(size + 1, 0);
            unsigned int const read = file->ReadBytes(&buffer[0], file->GetSize());
            file.reset();
            if (!read)
            {
                return -1;
            }
            buffer[read] = '\0';

            CStr const fileContent(&buffer[0]);
            retruxx::vector<CStr> tokens;
            Tokenize(&fileContent, tokens, "\r\n");
            size_t first = 0;
            // NOTE: only the first character is looked at, and a file that holds nothing but line breaks reads the
            // first entry of an empty list.
            if (!tokens.empty() && tokens[0].c_str()[0] == '1')
            {
                m_EnableMapping = true;
                first = 1;
            }
            else if (tokens.data()->c_str()[0] == '0')
            {
                m_EnableMapping = false;
                first = 1;
            }

            for (size_t i = first; i < tokens.size(); ++i)
            {
                CStr& token = tokens[i];
                DWORD const attr = ::GetFileAttributesA(token.c_str());
                if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) != 0)
                {
                    AddFolder(token.c_str(), "*.*", true);
                    continue;
                }

                retruxx::vector<CStr> parts;
                Tokenize(&token, parts, " \t");
                bool const enableMapping = m_EnableMapping;
                if (parts.size() > 1 && parts[1] == "M")
                {
                    m_EnableMapping = true;
                    token = parts[0];
                }
                size_t const length = token.c_str() ? strlen(token.c_str()) : 0;
                if (length >= 5 && !_stricmp(token.c_str() + length - 4, ".GDP"))
                {
                    InternalAddPackage(token);
                }
                else
                {
                    AddFile(token.c_str());
                }
                m_EnableMapping = enableMapping;
            }
            m_Initialized = true;
            return 0;
        }

        int FileServer::AddFolder(char const* FolderName, char const* FileMask, bool)
        {
            // RVA 0x664440 - NOTE: subfolders are always searched, with "*.*" whatever mask was asked for, and only a
            // trailing backslash counts as the folder's separator.
            char szPath[MAX_PATH];
            char szBuffer[MAX_PATH];
            strcpy(szPath, FolderName);
            if (FolderName[strlen(FolderName) - 1] != '\\')
            {
                strcat(szPath, "\\");
            }
            strcpy(szBuffer, szPath);
            strcat(szBuffer, FileMask);

            CFileEnum fileEnum;
            _finddata_t cFile;
            if (fileEnum.StartEnumeration(szBuffer, &cFile))
            {
                do
                {
                    if (strcmp(cFile.name, ".") && strcmp(cFile.name, ".."))
                    {
                        CStr fullFileName(szPath);
                        fullFileName += CStr(cFile.name);
                        if ((cFile.attrib & _A_SUBDIR) != 0)
                        {
                            AddFolder(fullFileName.c_str(), "*.*", true);
                        }
                        else
                        {
                            AddFile(fullFileName.c_str());
                        }
                    }
                } while (fileEnum.GetNextFile(&cFile));
            }
            return 0;
        }

        void FileServer::SetCurrentWorkDir(char const* currentDirectory)
        {
            // RVA 0x6624B0
            m_CurrentWorkDir = CStr(currentDirectory);
            UnifyFileName0(m_CurrentWorkDir);
            if (m_CurrentWorkDir.c_str()[strlen(m_CurrentWorkDir.c_str()) - 1] != '/')
            {
                m_CurrentWorkDir += CStr("/");
            }
        }

        void FileServer::DecryptFileName(char const* fileName, CStr& decryptedFileName)
        {
            // RVA 0x6627B0 - a leading ':' makes the name relative to the current work folder.
            if (fileName[0] == ':')
            {
                decryptedFileName = m_CurrentWorkDir + CStr(fileName + 1);
            }
            else
            {
                decryptedFileName = CStr(fileName);
            }
            UnifyFileName0(decryptedFileName);
        }

        int FileServer::InternalAddPackage(CStr const& PackageName)
        {
            // RVA 0x6650D0 - NOTE: a package that fails to open is leaked.
            Package* const package = new Package(m_EnableMapping);
            if (package->Open(PackageName.c_str()))
            {
                return -1;
            }
            m_Packages.push_back(PackagePrt(package));
            return 0;
        }

        int FileServer::EnumDataFolderFiles(char const* folder)
        {
            // NOTE: declared in the PDB but never emitted, so there is no original body; it is taken to add the
            // folder's files the way Initialize does for a folder entry.
            return AddFolder(folder, "*.*", true);
        }

        char const* FileServer::GetCurrentWorkDir() const
        {
            // RVA 0x662550
            return m_CurrentWorkDir.c_str();
        }

        void FileServer::GetOpenFilesList(retruxx::vector<CStr>& fileList) const
        {
            // RVA 0x664760
            fileList.clear();
            for (auto const& package : m_Packages)
            {
                retruxx::vector<CStr> tmpList;
                package->GetOpenFilesList(tmpList);
                fileList.insert(fileList.end(), tmpList.begin(), tmpList.end());
            }
        }
    }  // namespace fs
}  // namespace m3d
