#pragma once
#include "rapidjson/include/document.h"
#include "rapidjson/include/prettywriter.h"
#include "../../Utils.h"
#include "../ReferenceCounter.h"
#include "../DatOwner.h"
#include "RmPtfxShader.h"
#include "PtxRulePropList.h"
#include "PtxRenderState.h"
#include "PtxTriggerEvent.h"

namespace rage
{
    class ptxRule : public atReferenceCounter
    {
    public:
        ptxRule(const char* className);
        
        ptxRule(const datResource& rsc) : mSpawnEffectA(rsc), mSpawnEffectB(rsc)
        {
            rsc.PointerFixUp(mName);
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        virtual void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) = 0;
        virtual void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) = 0;

        void Place(void* that, const datResource& rsc);

        uint32_t GetObjectSize() const
        {
            return 0x6A0;
        }

        int32_t mUIData;
        float mFileVersion;
        int32_t field_10;
        int8_t field_14[8];
        int8_t field_1C;
        ptxTriggerEvent mSpawnEffectA;
        ptxTriggerEvent mSpawnEffectB;
        ptxRenderState mRenderState;
        float mPhysicalRange;
        float mStopVelocity;
        uint32_t mFlags;
        int32_t field_120;
        char* mName;
        char mClassName[10];
        int8_t mPercentPhysical;
        int8_t mPercentKill;
        int8_t field_134[0x9];

    protected:
        static const char* PtxCullModeToString(ePtxCullMode cull);
        static ePtxCullMode StringToPtxCullMode(const char* name);

        void WriteToJsonBase(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
        void LoadFromJsonBase(rapidjson::GenericObject<true, rapidjson::Value>& object);
    };
    ASSERT_SIZE(ptxRule, 0x140);


    class ptxSprite : public ptxRule
    {
    public:
        ptxSprite();

        ptxSprite(const datResource& rsc) : ptxRule(rsc), mClipRegionData(rsc), mProps(rsc), mShader(rsc) {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mClipRegionData.AddToLayout(layout, depth);
            mProps.AddToLayout(layout, depth);
            mShader.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mClipRegionData.SerializePtrs(layout, rsc, depth);
            mProps.SerializePtrs(layout, rsc, depth);
            mShader.SerializePtrs(layout, rsc, depth);
        }

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) override;

        float mFlipChanceU;
        float mFlipChanceV;
        float mNearClipDist;
        int8_t mIsScreenSpace;
        float mFarClipDist;
        float mProjectionDepth;
        float mShadowCastIntensity;
        int8_t mIsHiRes;
        Vector3 mAlignAxis;
        int8_t mAlignmentMode;
        int32_t mStartTexFrameIndex;
        int32_t mEndTexFrameIndex;
        int32_t mEndTexAnimFrame;
        int32_t mNumTextureTilesX;
        int32_t mNumTextureTilesY;
        //unknown type - always null
        datOwner<void*> mClipRegionData;
        ptxSpriteRulePropList mProps;
        rmPtfxShader mShader;
        float mTrimCornersAmount;
        bool mTrimCorners;
        int8_t field_69D[2];
        int8_t field_69F;
    };
    ASSERT_SIZE(ptxSprite, 0x6A0);


    class ptxModel : public ptxRule
    {
    public:
        ptxModel();

        ptxModel(const datResource& rsc) : ptxRule(rsc), mDrawables(rsc), mProps(rsc) {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mDrawables.AddToLayout(layout, depth);
            mProps.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mDrawables.SerializePtrs(layout, rsc, depth);
            mProps.SerializePtrs(layout, rsc, depth);
        }

        void AssignDrawables(pgDictionary<rmcDrawable>& drawables);

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) override;

        Vector3 mRotation;
        int8_t field_14C[4];
        Vector3 mRotationVar;
        int8_t field_15C[4];
        Vector2 mRotationSpeedVar;
        atArray<PtxNameDrawablePair> mDrawables;
        void* field_170;
        ptxModelRulePropList mProps;
        int8_t field_698[7];
        int8_t mDisableDraw;
    };
    ASSERT_SIZE(ptxModel, 0x6A0);
}