#pragma once
#include "rapidjson/include/prettywriter.h"
#include "../../Utils.h"
#include "../ReferenceCounter.h"
#include "PtxEvolution.h"
#include "PtxKeyFrame.h"
#include "PtxTimeline.h"

namespace rage
{
    class ptxEffectRule : public atReferenceCounter
    {
    public:
        ptxEffectRule() : mName(nullptr), mNumLoops(1), mPtxEvoGroup(nullptr), mZoomLevel(0.0f), mPUIData(nullptr), mColourTintMaxEnable(1), mUseRandomColor(false),
                          mUseDefaultFunctors(false), field_10F{}, mUseDataVolume(false), mDataVolumeType(0), mGameFlags(0), mNumActiveInstances(0), field_118{}, field_11F() {}

        ptxEffectRule(const datResource& rsc) : mKFColorTint(rsc), mKFColorTintMax(rsc), mKFZoom(rsc), mKFRotation(rsc), mKFDataSphere(rsc),
                                                mKFDataCapsuleA(rsc), mPtxEvoGroup(rsc)
        {
            if(mName)
            {
                rsc.PointerFixUp(mName);
            }
        }

        ~ptxEffectRule()
        {
            if(mName)
            {
                delete[] mName;
                mName = nullptr;
            }

            if(mPtxEvoGroup.Get())
            {
                delete mPtxEvoGroup.Get();
                mPtxEvoGroup = {nullptr};
            }
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        uint32_t GetObjectSize() const
        {
            return 0x1A0;
        }

        virtual void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) = 0;
        virtual void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) = 0;

        void Place(void* that, const datResource& rsc);

        rmPtfxKeyframe mKFColorTint;
        rmPtfxKeyframe mKFColorTintMax;
        rmPtfxKeyframe mKFZoom;
        rmPtfxKeyframe mKFRotation;
        rmPtfxKeyframe mKFDataSphere;
        rmPtfxKeyframe mKFDataCapsuleA;
        char* mName;
        int32_t mNumLoops;
        datOwner<ptxEvolutionGroup> mPtxEvoGroup;
        float mZoomLevel;
        void* mPUIData;
        int8_t mColourTintMaxEnable;
        bool mUseRandomColor;
        bool mUseDefaultFunctors;
        int8_t field_10F[2];
        bool mUseDataVolume;
        int8_t mDataVolumeType;
        uint8_t mGameFlags;
        int32_t mNumActiveInstances;
        int8_t field_118[7];
        int8_t field_11F;

    protected:
        void WriteToJsonBase(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
        void LoadFromJsonBase(rapidjson::GenericObject<true, rapidjson::Value>& object);
    };
    ASSERT_SIZE(ptxEffectRule, 0x120);


    class ptxEffectRuleStd : public ptxEffectRule
    {
    public:
        ptxEffectRuleStd() : ptxEffectRule(), mDistanceCullingFadeDist(-1.0f), mViewportCullingSphereRadius(0.0f), mDistanceCullingCullDist(-1.0f), mLodEvoDistMin(-1.0f), mLodEvoDistMax(-1.0f),
                             mFileVersion(4.0f), mDurationMin(1.0f), mDurationMax(1.0f), mPlaybackRateScalarMin(1.0f), mPlaybackRateScalarMax(1.0f), mPreUpdateTime(0), mPreUpdateTimeInterval(0),
                             mUseCullSphere(false), mCullNoUpdate(false), mCullNoEmit(false), mCullNoDraw(true), mSortEventsByDistance(false), mDrawListId(0), field_17A{},
                             mCullSphere(0.0f), mColnRange(0.0f), mColnProbeDist(0.0f), field_18C{}, mRandomOffsetPos{.x = 0.0f, .y = 0.0f, .z = 0.0f}, field_19C{}, field_19F(0)
        {
            mTimeline.mEffectRule = {this};
        }

        ptxEffectRuleStd(const datResource& rsc) : ptxEffectRule(rsc), mTimeline(rsc)
        {
            mPreUpdateTimeInterval = 0;
            mDrawListId = mDrawListId == 0 ? mDrawListId : 0;
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) override;

        ptxTimeLine mTimeline;
        float mDistanceCullingFadeDist;
        float mViewportCullingSphereRadius;
        float mDistanceCullingCullDist;
        float mLodEvoDistMin;
        float mLodEvoDistMax;
        float mFileVersion;
        float mDurationMin;
        float mDurationMax;
        float mPlaybackRateScalarMin;
        float mPlaybackRateScalarMax;
        int32_t mPreUpdateTime;
        int32_t mPreUpdateTimeInterval;
        bool mUseCullSphere;
        bool mCullNoUpdate;
        bool mCullNoEmit;
        bool mCullNoDraw;
        bool mSortEventsByDistance;
        int8_t mDrawListId;
        int8_t field_17A[4];
        float mCullSphere;
        float mColnRange;
        float mColnProbeDist;
        int8_t field_18C[4];
        Vector3 mRandomOffsetPos;
        int8_t field_19C[3];
        int8_t field_19F;
    };
    ASSERT_SIZE(ptxEffectRuleStd, 0x1A0);
}