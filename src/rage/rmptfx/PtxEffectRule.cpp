#include "PtxEffectRule.h"
#include "JsonHelpers.h"

namespace rage
{
    void ptxEffectRule::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mKFColorTint.AddToLayout(layout, depth);
        mKFColorTintMax.AddToLayout(layout, depth);
        mKFZoom.AddToLayout(layout, depth);
        mKFRotation.AddToLayout(layout, depth);
        mKFDataSphere.AddToLayout(layout, depth);
        mKFDataCapsuleA.AddToLayout(layout, depth);

        if(mName)
            layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);

        mPtxEvoGroup.AddToLayout(layout, depth);

        ((ptxEffectRuleStd*)this)->AddToLayout(layout, depth);
    }

    void ptxEffectRule::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mKFColorTint.SerializePtrs(layout, rsc,  depth);
        mKFColorTintMax.SerializePtrs(layout, rsc,  depth);
        mKFZoom.SerializePtrs(layout, rsc,  depth);
        mKFRotation.SerializePtrs(layout, rsc,  depth);
        mKFDataSphere.SerializePtrs(layout, rsc,  depth);
        mKFDataCapsuleA.SerializePtrs(layout, rsc,  depth);

        if(mName)
            layout.SerializePtr(mName, strlen(mName) + 1);

        mPtxEvoGroup.SerializePtrs(layout, rsc,  depth);

        ((ptxEffectRuleStd*)this)->SerializePtrs(layout, rsc, depth);
    }

    void ptxEffectRule::Place(void* that, const datResource& rsc)
    {
        new(that) ptxEffectRuleStd(rsc);
    }

    void ptxEffectRule::WriteToJsonBase(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        if(mName)
        {
            writer.String("Name");
            writer.String(mName);
        }

        writer.String("Version");
        writer.Uint(100);

        writer.String("KFColorTint");
        mKFColorTint.WriteToJson(writer);
        writer.String("KFColorTintMax");
        mKFColorTintMax.WriteToJson(writer);
        writer.String("KFZoom");
        mKFZoom.WriteToJson(writer);
        writer.String("KFRotation");
        mKFRotation.WriteToJson(writer);
        writer.String("KFDataSphere");
        mKFDataSphere.WriteToJson(writer);
        writer.String("KFDataCapsuleA");
        mKFDataCapsuleA.WriteToJson(writer);

        writer.String("NumLoops");
        writer.Int(mNumLoops);

        if(mPtxEvoGroup.Get())
        {
            writer.String("PtxEvoGroup");
            mPtxEvoGroup->WriteToJson(writer);
        }

        writer.String("ZoomLevel");
        writer.Double((double)mZoomLevel);

        writer.String("ColourTintMaxEnable");
        writer.Int(mColourTintMaxEnable);

        writer.String("UseRandomColor");
        writer.Bool(mUseRandomColor);
        writer.String("UseDefaultFunctors");
        writer.Bool(mUseDefaultFunctors);
        writer.String("UseDataVolume");
        writer.Bool(mUseDataVolume);

        writer.String("DataVolumeType");
        writer.Int(mDataVolumeType);

        writer.String("GameFlags");
        writer.Uint(mGameFlags);

        writer.String("NumActiveInstances");
        writer.Int(mNumActiveInstances);
        writer.String("field_11F");
        writer.Int(field_11F);
    }

    void ptxEffectRule::LoadFromJsonBase(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        if(object.HasMember("Name"))
        {
            mName = strdup(object["Name"].GetString());
        }

        JsonHelpers::LoadMemberObject(mKFColorTint, object, "KFColorTint");
        JsonHelpers::LoadMemberObject(mKFColorTintMax, object, "KFColorTintMax");
        JsonHelpers::LoadMemberObject(mKFZoom, object, "KFZoom");
        JsonHelpers::LoadMemberObject(mKFRotation, object, "KFRotation");
        JsonHelpers::LoadMemberObject(mKFDataSphere, object, "KFDataSphere");
        JsonHelpers::LoadMemberObject(mKFDataCapsuleA, object, "KFDataCapsuleA");

        mNumLoops = object["NumLoops"].GetInt();

        if(object.HasMember("PtxEvoGroup") && object["PtxEvoGroup"].IsObject())
        {
            auto evoGroupObject = object["PtxEvoGroup"].GetObject();
            mPtxEvoGroup = {new ptxEvolutionGroup()};
            mPtxEvoGroup->LoadFromJson(evoGroupObject);
        }

        mZoomLevel = object["ZoomLevel"].GetFloat();

        mColourTintMaxEnable = object["ColourTintMaxEnable"].GetInt();

        mUseRandomColor = object["UseRandomColor"].GetBool();
        mUseDefaultFunctors = object["UseDefaultFunctors"].GetBool();
        mUseDataVolume = object["UseDataVolume"].GetBool();

        mDataVolumeType = object["DataVolumeType"].GetInt();

        mGameFlags = object["GameFlags"].GetUint();

        mNumActiveInstances = object["NumActiveInstances"].GetInt();
        field_11F = object["field_11F"].GetInt();
    }


    void ptxEffectRuleStd::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mTimeline.AddToLayout(layout, depth);
    }

    void ptxEffectRuleStd::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mTimeline.SerializePtrs(layout, rsc, depth);
    }

    void ptxEffectRuleStd::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            WriteToJsonBase(writer);

            writer.String("Timeline");
            mTimeline.WriteToJson(writer);

            writer.String("DistanceCullingFadeDist");
            writer.Double((double)mDistanceCullingFadeDist);
            writer.String("ViewportCullingSphereRadius");
            writer.Double((double)mViewportCullingSphereRadius);
            writer.String("DistanceCullingCullDist");
            writer.Double((double)mDistanceCullingCullDist);
            writer.String("LodEvoDistMin");
            writer.Double((double)mLodEvoDistMin);
            writer.String("LodEvoDistMax");
            writer.Double((double)mLodEvoDistMax);
            writer.String("DurationMin");
            writer.Double((double)mDurationMin);
            writer.String("DurationMax");
            writer.Double((double)mDurationMax);
            writer.String("PlaybackRateScalarMin");
            writer.Double((double)mPlaybackRateScalarMin);
            writer.String("PlaybackRateScalarMax");
            writer.Double((double)mPlaybackRateScalarMax);

            writer.String("PreUpdateTime");
            writer.Int(mPreUpdateTime);
            writer.String("PreUpdateTimeInterval");
            writer.Int(mPreUpdateTimeInterval);

            writer.String("UseCullSphere");
            writer.Bool(mUseCullSphere);
            writer.String("CullNoUpdate");
            writer.Bool(mCullNoUpdate);
            writer.String("CullNoEmit");
            writer.Bool(mCullNoEmit);
            writer.String("CullNoDraw");
            writer.Bool(mCullNoDraw);
            writer.String("SortEventsByDistance");
            writer.Bool(mSortEventsByDistance);

            writer.String("DrawListId");
            writer.Int(mDrawListId);

            writer.String("CullSphere");
            writer.Double((double)mCullSphere);
            writer.String("ColnRange");
            writer.Double((double)mColnRange);
            writer.String("ColnProbeDist");
            writer.Double((double)mColnProbeDist);

            writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
            writer.String("RandomOffsetPos");
            writer.StartArray();
            {
                writer.Double((double)mRandomOffsetPos.x);
                writer.Double((double)mRandomOffsetPos.y);
                writer.Double((double)mRandomOffsetPos.z);
            }
            writer.EndArray();
            writer.SetFormatOptions(rapidjson::kFormatDefault);

            writer.String("field_19F");
            writer.Int(field_19F);
        }
        writer.EndObject();
    }

    void ptxEffectRuleStd::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        LoadFromJsonBase(object);

        JsonHelpers::LoadMemberObject(mTimeline, object, "Timeline");

        mDistanceCullingFadeDist = object["DistanceCullingFadeDist"].GetFloat();
        mViewportCullingSphereRadius = object["ViewportCullingSphereRadius"].GetFloat();
        mDistanceCullingCullDist = object["DistanceCullingCullDist"].GetFloat();
        mLodEvoDistMin = object["LodEvoDistMin"].GetFloat();
        mLodEvoDistMax = object["LodEvoDistMax"].GetFloat();
        mDurationMin = object["DurationMin"].GetFloat();
        mDurationMax = object["DurationMax"].GetFloat();
        mPlaybackRateScalarMin = object["PlaybackRateScalarMin"].GetFloat();
        mPlaybackRateScalarMax = object["PlaybackRateScalarMax"].GetFloat();

        mPreUpdateTime = object["PreUpdateTime"].GetInt();
        mPreUpdateTimeInterval = object["PreUpdateTimeInterval"].GetInt();

        mUseCullSphere = object["UseCullSphere"].GetBool();
        mCullNoUpdate = object["CullNoUpdate"].GetBool();
        mCullNoEmit = object["CullNoEmit"].GetBool();
        mCullNoDraw = object["CullNoDraw"].GetBool();
        mSortEventsByDistance = object["SortEventsByDistance"].GetBool();

        mDrawListId = object["DrawListId"].GetInt();

        mCullSphere = object["CullSphere"].GetFloat();
        mColnRange = object["ColnRange"].GetFloat();
        mColnProbeDist = object["ColnProbeDist"].GetFloat();

        mRandomOffsetPos.x = object["RandomOffsetPos"].GetArray()[0].GetFloat();;
        mRandomOffsetPos.y = object["RandomOffsetPos"].GetArray()[1].GetFloat();;
        mRandomOffsetPos.z = object["RandomOffsetPos"].GetArray()[2].GetFloat();;

        field_19F = object["field_19F"].GetInt();
    }
}