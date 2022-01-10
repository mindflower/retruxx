#include <memory>
#include <stdexcept>
#include <core/ini.h>
#include <file/filereader.h>
#include <file/fileserver.h>
#include <file/memoryfile.h>
#include <file/package.h>
#include <file/rawfile.h>

namespace m3d
{
    namespace fs
    {
        FileServer::~FileServer()
        {
            Shutdown();
        }

        bool FileServer::FileExists(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream* FileServer::CreateFileStream()
        {
            return new FileReader;
        }

        int FileServer::RemoveFile(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        void FileServer::EnableMapping(bool)
        {
            throw std::logic_error("Not implemented");
        }

        int FileServer::Reinitialize(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        int FileServer::Shutdown()
        {
            throw std::logic_error("Not implemented");
        }

        CriticalSection& FileServer::GetCriticalSecton()
        {
            return m_cs;
        }

        int FileServer::OpenFileStream(FileReader* reader, char const* filename, IStream::OpenFlags flags)
        {
            //TODO: check this
            if (filename[1] == ':' && (filename[2] == '/' || filename[2] == '\\'))
            {
                CStr filenameUnified;
                UnifyFileName(filenameUnified);
                delete reader->InternalObject;
                reader->InternalObject = new RawFile(filenameUnified.c_str(), flags, m_EnableMapping);
                return reader->InternalObject->IsOpen();
            }
            CStr fullFilename;
            DecryptFileName(filename, fullFilename);

            if (flags == IStream::OPEN_READ)
            {
                for (auto const& package : m_Packages)
                {
                    if (package->HaveFile(fullFilename.c_str()))
                    {
                        delete reader->InternalObject;
                        reader->InternalObject = new MemoryFile(fullFilename.c_str(), package);
                        return 1;
                    }
                }
            }
            auto const it = m_Files.find(fullFilename);
            if (it == m_Files.cend())
            {
                m_Files.insert(fullFilename);
            }
            delete reader->InternalObject;
            reader->InternalObject = new RawFile(fullFilename.c_str(), flags, m_EnableMapping);
            if (!reader->InternalObject->IsOpen())
            {
                return 0;
            }
            return 1;
        }

        int FileServer::AddPackage(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        int FileServer::AddFile(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        int FileServer::RemoveFolder(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        int FileServer::Initialize(char const* dataSource)
        {
            if (m_Initialized)
            {
                return - 1;
            }

            auto file = std::make_unique<RawFile>(dataSource, IStream::OPEN_READ, m_EnableMapping);
            if (!file->IsOpen())
            {
                auto const res = file->Error();
                return res;
            } 

            auto const size = file->GetSize();
            std::vector<char> buffer(size + 1, 0);
            if (!file->ReadBytes(buffer.data(), size))
            {
                return -1;
            }
            file.reset();

            buffer[size] = '\0';
            CStr const fileContent(buffer.data());
            std::vector<CStr> tokens;
            Tokenize(&fileContent, tokens, "\r\n");
            if (!tokens.empty())
            {
                if (auto const& first = tokens.front(); first == "0")
                {
                    m_EnableMapping = false;
                }
                else if (first == "1")
                {
                    m_EnableMapping = true;
                }
                tokens.erase(tokens.begin());
            }
            for (auto& token : tokens)
            {
                auto attr = ::GetFileAttributesA(token.c_str());
                if (attr == -1 || (attr & 0x10) == 0)
                {
                    auto const enableMapping = m_EnableMapping;
                    std::vector<CStr> tokens2;
                    Tokenize(&token, tokens, " \t");
                    if (tokens2.size() > 1 && tokens2[1] == "M")
                    {
                        m_EnableMapping = true;
                        token = tokens2.front();
                    }
                    if (token.rfind(".GDP") != CStr::npos)
                    {
                        InternalAddPackage(token);
                    }
                    else
                    {
                        AddFile(token.c_str());
                    }
                    m_EnableMapping = enableMapping;
                }
                else
                {
                    AddFolder(token.c_str(), "*.*", true);
                }
            }
            m_Initialized = true;
            return 0;
        }

        int FileServer::AddFolder(char const*, char const*, bool)
        {
            throw std::logic_error("Not implemented");
        }

        void FileServer::SetCurrentWorkDir(char const* currentDirectory)
        {
            m_CurrentWorkDir = currentDirectory;
            UnifyFileName0(m_CurrentWorkDir);
            if (m_CurrentWorkDir.back() != '/')
            {
                m_CurrentWorkDir += '/';
            }
        }

        void FileServer::DecryptFileName(char const* fileName, CStr& decryptedFileName)
        {
            if (fileName[0] == ':')
            {
                CStr const temp(fileName + 1);
                decryptedFileName = m_CurrentWorkDir + temp;
            }
            decryptedFileName = fileName;
            UnifyFileName0(decryptedFileName);
        }

        int FileServer::InternalAddPackage(CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        char const* FileServer::GetCurrentWorkDir() const
        {
            return m_CurrentWorkDir.c_str();
        }

        void FileServer::GetOpenFilesList(std::vector<CStr>& fileList) const
        {
            fileList.clear();
            for (const auto& package : m_Packages)
            {
                std::vector<CStr> tmpList;
                package->GetOpenFilesList(tmpList);
                fileList.insert(end(fileList), begin(tmpList), end(tmpList));
            }
        }
    }
}
