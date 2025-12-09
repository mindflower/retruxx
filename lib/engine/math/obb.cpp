#include "math/obb.h"
#include <corecrt_math.h>
#include "math/aabb.h"
#include <utility>

void Obb::Create(const CVector& min, const CVector& max, const CMatrix& mat, bool noScale)
{
    // TODO: check this
    this->m_min = min;
    this->m_max = max;
    auto _42 = mat._42;
    auto _43 = mat._43;
    this->m_origin.x = mat._41;
    this->m_origin.y = _42;
    this->m_origin.z = _43;
    if (noScale)
    {
        mat.GetNormalizedBasis(m_basis[0], m_basis[1], m_basis[2]);
    }
    else
    {
        this->m_basis[0].x = mat._11;
        this->m_basis[0].y = mat._21;
        this->m_basis[0].z = mat._31;
        this->m_basis[1].x = mat._12;
        this->m_basis[1].y = mat._22;
        this->m_basis[1].z = mat._32;
        this->m_basis[2].x = mat._13;
        this->m_basis[2].y = mat._23;
        this->m_basis[2].z = mat._33;
        auto p_m_max = &this->m_max;
        auto p_z = &this->m_basis[0].z;
        auto v10 = 3;
        do
        {
            auto v11 = *p_z;
            auto v12 = *(p_z - 1);
            p_m_max = (CVector*)((char*)p_m_max + 4);
            auto v13 = *(p_z - 2);
            p_z += 3;
            --v10;
            auto v14 = sqrt(v13 * v13 + v12 * v12 + v11 * v11);
            auto noScalea = v14;
            p_m_max[-2].z = v14 * p_m_max[-2].z;
            p_m_max[-1].z = noScalea * p_m_max[-1].z;
            *(p_z - 5) = *(p_z - 5) * (float)(1.0 / noScalea);
            *(p_z - 4) = *(p_z - 4) * (float)(1.0 / noScalea);
            *(p_z - 3) = *(p_z - 3) * (float)(1.0 / noScalea);
        } while (v10);
    }
}

Aabb Obb::GetBounds() const
{
    // TODO: check and refactor this
    float y;       // xmm4_4
    float z;       // xmm5_4
    float v4;      // xmm0_4
    float v5;      // xmm2_4
    float v6;      // xmm3_4
    float v7;      // xmm1_4
    float v8;      // xmm2_4
    float x;       // xmm3_4
    float v10;     // xmm0_4
    float v11;     // xmm0_4
    float v12;     // xmm1_4
    float v13;     // xmm2_4
    float v14;     // xmm5_4
    float v15;     // xmm4_4
    float v16;     // xmm2_4
    float v17;     // xmm5_4
    float v18;     // edx
    float v19;     // xmm0_4
    float v20;     // xmm2_4
    float v21;     // xmm4_4
    float v22;     // xmm1_4
    float v23;     // xmm0_4
    float v24;     // eax
    float v25;     // xmm2_4
    float v26;     // xmm3_4
    float v27;     // xmm1_4
    float v28;     // xmm2_4
    float v29;     // xmm5_4
    float v30;     // edx
    float v31;     // xmm1_4
    float v32;     // xmm0_4
    float v33;     // xmm2_4
    float v34;     // xmm5_4
    float v35;     // xmm2_4
    float v36;     // xmm3_4
    float v37;     // xmm1_4
    float v38;     // xmm2_4
    float v39;     // xmm3_4
    float v40;     // xmm0_4
    float v41;     // xmm1_4
    float v42;     // xmm2_4
    float v43;     // xmm3_4
    float v44;     // xmm0_4
    float v45;     // xmm1_4
    float v46;     // xmm0_4
    float v47;     // eax
    float v48;     // xmm1_4
    float v49;     // xmm3_4
    float v50;     // xmm2_4
    float v51;     // xmm5_4
    float v52;     // edx
    float v53;     // xmm5_4
    float v54;     // xmm4_4
    float v55;     // xmm2_4
    float v56;     // xmm5_4
    float v57;     // xmm2_4
    float v58;     // xmm3_4
    float v59;     // xmm1_4
    float v60;     // xmm0_4
    float v61;     // xmm1_4
    float v62;     // xmm2_4
    float v63;     // xmm5_4
    float v64;     // xmm4_4
    float v65;     // xmm1_4
    float v66;     // xmm2_4
    float v67;     // xmm5_4
    float v68;     // xmm4_4
    float v69;     // xmm3_4
    float v70;     // xmm2_4
    float v71;     // xmm4_4
    float v72;     // xmm2_4
    float v73;     // xmm0_4
    float v74;     // xmm1_4
    float v75;     // eax
    float v76;     // xmm2_4
    float v77;     // xmm0_4
    float v78;     // xmm1_4
    float v79;     // xmm0_4
    float v80;     // xmm1_4
    float v81;     // xmm2_4
    float v82;     // xmm5_4
    float v83;     // xmm4_4
    float v84;     // xmm2_4
    float v85;     // xmm5_4
    float v86;     // xmm1_4
    float v87;     // xmm0_4
    float v88;     // xmm4_4
    float v89;     // xmm3_4
    float v90;     // xmm2_4
    float v91;     // xmm4_4
    float v92;     // xmm0_4
    float v93;     // xmm1_4
    float v94;     // xmm2_4
    float v95;     // xmm5_4
    float v96;     // xmm4_4
    float v97;     // xmm2_4
    int i;         // ebx
    float* v99;    // esi
    float* v100;   // edx
    float* v101;   // ecx
    float* v102;   // eax
    bool v103;     // cc
    float* v104;   // eax
    float* v105;   // edx
    float* v106;   // edx
    float* v107;   // edx
    float* v108;   // edi
    float* v109;   // edx
    float* v110;   // edi
    float* v111;   // edx
    float* v112;   // edi
    float* v113;   // edx
    float* v114;   // edi
    float* v115;   // edx
    float* v116;   // esi
    float v118;    // xmm0_4
    float v119;    // ecx
    float v120;    // xmm0_4
    float v121;    // ecx
    float v122;    // xmm0_4
    float v123;    // ecx
    float v124;    // xmm0_4
    CVector maxV;  // [esp+Ch] [ebp-90h] BYREF
    CVector minV;  // [esp+18h] [ebp-84h] BYREF
    Aabb box;      // [esp+24h] [ebp-78h]
    CVector v[8];  // [esp+3Ch] [ebp-60h] BYREF

    y = this->m_min.y;
    z = this->m_max.z;
    v4 = (float)((float)((float)(this->m_basis[0].z * this->m_min.z) + (float)(this->m_basis[0].y * y)) + (float)(this->m_min.x * this->m_basis[0].x)) +
        this->m_origin.x;
    v5 = (float)(this->m_basis[2].z * this->m_min.z) + (float)(this->m_basis[2].y * y);
    v6 = this->m_min.x * this->m_basis[2].x;
    maxV.y = (float)((float)((float)(this->m_basis[1].z * this->m_min.z) + (float)(this->m_basis[1].y * y)) + (float)(this->m_min.x * this->m_basis[1].x)) +
        this->m_origin.y;
    v7 = this->m_basis[0].y * y;
    v8 = (float)(v5 + v6) + this->m_origin.z;
    x = this->m_min.x;
    maxV.x = v4;
    v10 = this->m_basis[0].z;
    maxV.z = v8;
    v11 = (float)((float)((float)(v10 * z) + v7) + (float)(this->m_basis[0].x * x)) + this->m_origin.x;
    v12 = (float)((float)((float)(this->m_basis[1].z * z) + (float)(this->m_basis[1].y * y)) + (float)(this->m_basis[1].x * x)) + this->m_origin.y;
    v13 = this->m_basis[2].z * z;
    v14 = this->m_basis[2].y * y;
    v15 = this->m_basis[2].x;
    v16 = v13 + v14;
    v17 = this->m_max.z;
    v[0].x = maxV.x;
    v[0].y = maxV.y;
    maxV.x = v11;
    v18 = v11;
    v19 = this->m_basis[0].z;
    v20 = (float)(v16 + (float)(v15 * x)) + this->m_origin.z;
    v21 = this->m_max.y;
    v[0].z = maxV.z;
    maxV.y = v12;
    maxV.z = v20;
    v[1].x = v18;
    v[1].y = v12;
    v[1].z = v20;
    v22 = (float)(this->m_basis[1].z * v17) + (float)(this->m_basis[1].y * v21);
    maxV.x = (float)((float)((float)(v19 * v17) + (float)(this->m_basis[0].y * v21)) + (float)(x * this->m_basis[0].x)) + this->m_origin.x;
    v23 = this->m_basis[0].z * this->m_max.z;
    v24 = maxV.x;
    v25 = x * this->m_basis[1].x;
    v26 = x * this->m_basis[2].x;
    v27 = (float)(v22 + v25) + this->m_origin.y;
    v28 = this->m_basis[2].z * v17;
    v29 = this->m_basis[2].y;
    maxV.y = v27;
    v30 = v27;
    v31 = this->m_basis[1].z * this->m_max.z;
    v32 = (float)((float)(v23 + (float)(this->m_basis[0].y * v21)) + (float)(this->m_basis[0].x * this->m_max.x)) + this->m_origin.x;
    v33 = v28 + (float)(v29 * v21);
    v34 = this->m_min.z;
    v35 = (float)(v33 + v26) + this->m_origin.z;
    v36 = this->m_basis[2].y;
    maxV.z = v35;
    v37 = (float)((float)(v31 + (float)(this->m_basis[1].y * v21)) + (float)(this->m_basis[1].x * this->m_max.x)) + this->m_origin.y;
    v38 = (float)(this->m_basis[2].z * this->m_max.z) + (float)(v36 * v21);
    v39 = this->m_basis[2].x * this->m_max.x;
    maxV.x = v32;
    v40 = this->m_basis[0].z;
    maxV.y = v37;
    v41 = this->m_basis[0].y;
    v[2].x = v24;
    v[2].y = v30;
    v42 = (float)(v38 + v39) + this->m_origin.z;
    v43 = this->m_max.x;
    v[2].z = maxV.z;
    maxV.z = v42;
    v[3].x = maxV.x;
    v44 = (float)((float)((float)(v40 * v34) + (float)(v41 * v21)) + (float)(v43 * this->m_basis[0].x)) + this->m_origin.x;
    v[3].y = maxV.y;
    v[3].z = v42;
    v45 = (float)(this->m_basis[1].z * v34) + (float)(this->m_basis[1].y * v21);
    maxV.x = v44;
    v46 = this->m_basis[0].z;
    v47 = maxV.x;
    v48 = (float)(v45 + (float)(v43 * this->m_basis[1].x)) + this->m_origin.y;
    v49 = v43 * this->m_basis[2].x;
    v50 = this->m_basis[2].z * v34;
    v51 = this->m_basis[2].y;
    maxV.y = v48;
    v52 = v48;
    v53 = v51 * v21;
    v54 = this->m_min.y;
    v55 = v50 + v53;
    v56 = this->m_min.z;
    v57 = (float)(v55 + v49) + this->m_origin.z;
    v58 = this->m_max.x;
    v59 = this->m_basis[1].z;
    v60 = (float)((float)((float)(v46 * v56) + (float)(this->m_basis[0].y * v54)) + (float)(this->m_basis[0].x * v58)) + this->m_origin.x;
    maxV.z = v57;
    v61 = (float)((float)((float)(v59 * v56) + (float)(this->m_basis[1].y * v54)) + (float)(this->m_basis[1].x * v58)) + this->m_origin.y;
    v62 = this->m_basis[2].z * v56;
    v63 = this->m_basis[2].y * v54;
    v64 = this->m_basis[2].x;
    maxV.y = v61;
    v65 = this->m_basis[0].y;
    v66 = v62 + v63;
    v67 = this->m_min.z;
    maxV.x = v60;
    v68 = v64 * v58;
    v69 = this->m_min.x;
    v70 = v66 + v68;
    v71 = this->m_max.y;
    v72 = v70 + this->m_origin.z;
    v73 = (float)(this->m_basis[0].z * v67) + (float)(v65 * v71);
    v74 = this->m_basis[0].x;
    v[4].x = v47;
    v75 = maxV.z;
    v[4].y = v52;
    maxV.z = v72;
    v76 = this->m_basis[1].y;
    v77 = v73 + (float)(v74 * v69);
    v78 = this->m_basis[1].z;
    v79 = v77 + this->m_origin.x;
    v[4].z = v75;
    v[5] = maxV;
    v80 = (float)((float)((float)(v78 * v67) + (float)(v76 * v71)) + (float)(this->m_basis[1].x * v69)) + this->m_origin.y;
    v81 = this->m_basis[2].z * v67;
    v82 = this->m_basis[2].y * v71;
    v83 = this->m_basis[2].x;
    v84 = v81 + v82;
    v85 = this->m_max.z;
    maxV.y = v80;
    v86 = this->m_basis[0].y;
    maxV.x = v79;
    v87 = this->m_basis[0].z;
    v88 = v83 * v69;
    v89 = this->m_max.x;
    v90 = (float)(v84 + v88) + this->m_origin.z;
    v91 = this->m_min.y;
    maxV.z = v90;
    v92 = (float)((float)((float)(v87 * v85) + (float)(v86 * v91)) + (float)(this->m_basis[0].x * v89)) + this->m_origin.x;
    v93 = (float)((float)((float)(this->m_basis[1].z * v85) + (float)(this->m_basis[1].y * v91)) + (float)(this->m_basis[1].x * v89)) + this->m_origin.y;
    v94 = this->m_basis[2].z * v85;
    v95 = this->m_basis[2].y * v91;
    v96 = this->m_basis[2].x;
    v[6].x = maxV.x;
    v97 = (float)((float)(v94 + v95) + (float)(v96 * v89)) + this->m_origin.z;
    v[6].y = maxV.y;
    v[6].z = maxV.z;
    v[7].x = v92;
    v[7].y = v93;
    v[7].z = v97;
    maxV = v[0];
    minV = v[0];
    for (i = 0; i < 12; i += 4)
    {
        v99 = (float*)((char*)&v[2].x + i);
        v100 = (float*)((char*)&v[1].x + i);
        v101 = (float*)((char*)&maxV.x + i);
        v102 = v100;
        if (*v100 <= *(float*)((char*)&maxV.x + i))
            v102 = (float*)((char*)&maxV.x + i);
        v103 = *(float*)((char*)&minV.x + i) <= *v100;
        *v101 = *v102;
        v104 = (float*)((char*)&minV.x + i);
        if (v103)
            v100 = (float*)((char*)&minV.x + i);
        v103 = *v99 <= *v101;
        *v104 = *v100;
        v105 = (float*)((char*)&v[2].x + i);
        if (v103)
            v105 = (float*)((char*)&maxV.x + i);
        v103 = *v104 <= *v99;
        *v101 = *v105;
        v106 = (float*)((char*)&v[2].x + i);
        if (v103)
            v106 = (float*)((char*)&minV.x + i);
        v103 = v99[3] <= *v101;
        *v104 = *v106;
        v107 = v99 + 3;
        v108 = v99 + 3;
        if (v103)
            v108 = (float*)((char*)&maxV.x + i);
        v103 = *v104 <= *v107;
        *v101 = *v108;
        if (v103)
            v107 = (float*)((char*)&minV.x + i);
        v103 = v99[6] <= *v101;
        *v104 = *v107;
        v109 = v99 + 6;
        v110 = v99 + 6;
        if (v103)
            v110 = (float*)((char*)&maxV.x + i);
        v103 = *v104 <= *v109;
        *v101 = *v110;
        if (v103)
            v109 = (float*)((char*)&minV.x + i);
        v103 = v99[9] <= *v101;
        *v104 = *v109;
        v111 = v99 + 9;
        v112 = v99 + 9;
        if (v103)
            v112 = (float*)((char*)&maxV.x + i);
        v103 = *v104 <= *v111;
        *v101 = *v112;
        if (v103)
            v111 = (float*)((char*)&minV.x + i);
        v103 = v99[12] <= *v101;
        *v104 = *v111;
        v113 = v99 + 12;
        v114 = v99 + 12;
        if (v103)
            v114 = (float*)((char*)&maxV.x + i);
        v103 = *v104 <= *v113;
        *v101 = *v114;
        if (v103)
            v113 = (float*)((char*)&minV.x + i);
        v103 = v99[15] <= *v101;
        *v104 = *v113;
        v115 = v99 + 15;
        v116 = v99 + 15;
        if (v103)
            v116 = (float*)((char*)&maxV.x + i);
        v103 = *v104 <= *v115;
        *v101 = *v116;
        if (v103)
            v115 = (float*)((char*)&minV.x + i);
        *v104 = *v115;
    }
    box.m_box[1] = minV.y;
    v118 = minV.z;

    Aabb result;
    result.m_box[0] = minV.x;
    result.m_box[1] = box.m_box[1];
    v119 = v118;
    v120 = maxV.x;
    result.m_box[2] = v119;
    v121 = v120;
    v122 = maxV.y;
    result.m_box[3] = v121;
    v123 = v122;
    v124 = maxV.z;
    result.m_box[4] = v123;
    result.m_box[5] = v124;
    return result;
}
