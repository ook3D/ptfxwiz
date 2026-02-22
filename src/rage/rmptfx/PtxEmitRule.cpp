#include "PtxEmitRule.h"
#include "JsonHelpers.h"

namespace rage
{
    void ptxEmitRuleStd::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mEmitterData.AddToLayout(layout, depth);

        for(size_t i = 0; i < std::size(mKeyFrames); i++)
        {
            mKeyFrames[i].AddToLayout(layout, depth);
        }

        if(mName)
            layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);
    }

    void ptxEmitRuleStd::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mEmitterData.SerializePtrs(layout, rsc, depth);

        for(size_t i = 0; i < std::size(mKeyFrames); i++)
        {
            mKeyFrames[i].SerializePtrs(layout, rsc, depth);
        }

        if(mName)
            layout.SerializePtr(mName, strlen(mName) + 1);
    }

    void ptxEmitRuleStd::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            writer.String("Name");
            writer.String(mName);

            writer.String("Version");
            writer.Uint(100);

            writer.String("Duration");
            writer.Double((double)mDuration);

            writer.String("EmitterData");
            mEmitterData.WriteToJson(writer);

            writer.String("mLastEvoList");
            writer.Int(mLastEvoList);

            writer.String("OneShot");
            writer.Bool(mOneShot);
        }
        writer.EndObject();
    }

    void ptxEmitRuleStd::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        mName = strdup(object["Name"].GetString());
        mDuration = object["Duration"].GetFloat();

        JsonHelpers::LoadMemberObject(mEmitterData, object, "EmitterData");

        mLastEvoList = object["mLastEvoList"].GetInt();
        mOneShot = object["OneShot"].GetBool();
    }

    void ptxEmitRuleStd::stdEmitterData::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mEmitterDomain.AddToLayout(layout, depth);
        mVelocityDomain.AddToLayout(layout, depth);

        mSpawnRateKF.AddToLayout(layout, depth);
        mSpawnDistKF.AddToLayout(layout, depth);
        mTimeScaleKF.AddToLayout(layout, depth);
        mSpawnLifeKF.AddToLayout(layout, depth);
        mSpeedKF.AddToLayout(layout, depth);
        mSizeKFOT.AddToLayout(layout, depth);
        mAccelerationKFOT.AddToLayout(layout, depth);
        mDampeningKFOT.AddToLayout(layout, depth);
        mMatrixWeightKFOT.AddToLayout(layout, depth);
        mInheritVelKFOT.AddToLayout(layout, depth);
    }

    void ptxEmitRuleStd::stdEmitterData::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mEmitterDomain.SerializePtrs(layout, rsc, depth);
        mVelocityDomain.SerializePtrs(layout, rsc, depth);

        mSpawnRateKF.SerializePtrs(layout, rsc, depth);
        mSpawnDistKF.SerializePtrs(layout, rsc, depth);
        mTimeScaleKF.SerializePtrs(layout, rsc, depth);
        mSpawnLifeKF.SerializePtrs(layout, rsc, depth);
        mSpeedKF.SerializePtrs(layout, rsc, depth);
        mSizeKFOT.SerializePtrs(layout, rsc, depth);
        mAccelerationKFOT.SerializePtrs(layout, rsc, depth);
        mDampeningKFOT.SerializePtrs(layout, rsc, depth);
        mMatrixWeightKFOT.SerializePtrs(layout, rsc, depth);
        mInheritVelKFOT.SerializePtrs(layout, rsc, depth);
    }

    void ptxEmitRuleStd::stdEmitterData::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            if(mEmitterDomain.Get())
            {
                writer.String("EmitterDomain");
                mEmitterDomain->WriteToJson(writer);
            }

            if(mVelocityDomain.Get())
            {
                writer.String("VelocityDomain");
                mVelocityDomain->WriteToJson(writer);
            }

            writer.String("SpawnRateKF");
            mSpawnRateKF.WriteToJson(writer);
            writer.String("SpawnDistKF");
            mSpawnDistKF.WriteToJson(writer);
            writer.String("TimeScaleKF");
            mTimeScaleKF.WriteToJson(writer);
            writer.String("SpawnLifeKF");
            mSpawnLifeKF.WriteToJson(writer);
            writer.String("SpeedKF");
            mSpeedKF.WriteToJson(writer);
            writer.String("SizeKFOT");
            mSizeKFOT.WriteToJson(writer);
            writer.String("AccelerationKFOT");
            mAccelerationKFOT.WriteToJson(writer);
            writer.String("DampeningKFOT");
            mDampeningKFOT.WriteToJson(writer);
            writer.String("MatrixWeightKFOT");
            mMatrixWeightKFOT.WriteToJson(writer);
            writer.String("InheritVelKFOT");
            mInheritVelKFOT.WriteToJson(writer);
        }
        writer.EndObject();
    }

    void ptxEmitRuleStd::stdEmitterData::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        if(object.HasMember("EmitterDomain") && object["EmitterDomain"].IsObject())
        {
            auto domainObject = object["EmitterDomain"].GetObject();
            ptxDomain::eDomainShape shape = ptxDomain::StringToShape(domainObject["Shape"].GetString());
            mEmitterDomain = CreateDomain(0, shape);
            mEmitterDomain->LoadFromJson(domainObject);
        }

        if(object.HasMember("VelocityDomain") && object["VelocityDomain"].IsObject())
        {
            auto domainObject = object["VelocityDomain"].GetObject();
            ptxDomain::eDomainShape shape = ptxDomain::StringToShape(domainObject["Shape"].GetString());
            mVelocityDomain = CreateDomain(1, shape);
            mVelocityDomain->LoadFromJson(domainObject);
        }

        JsonHelpers::LoadMemberObject(mSpawnRateKF, object, "SpawnRateKF");

        JsonHelpers::LoadMemberObject(mSpawnDistKF, object, "SpawnDistKF");
        JsonHelpers::LoadMemberObject(mTimeScaleKF, object, "TimeScaleKF");
        JsonHelpers::LoadMemberObject(mSpawnLifeKF, object, "SpawnLifeKF");
        JsonHelpers::LoadMemberObject(mSpeedKF, object, "SpeedKF");
        JsonHelpers::LoadMemberObject(mSizeKFOT, object, "SizeKFOT");
        JsonHelpers::LoadMemberObject(mAccelerationKFOT, object, "AccelerationKFOT");
        JsonHelpers::LoadMemberObject(mDampeningKFOT, object, "DampeningKFOT");
        JsonHelpers::LoadMemberObject(mMatrixWeightKFOT, object, "MatrixWeightKFOT");
        JsonHelpers::LoadMemberObject(mInheritVelKFOT, object, "InheritVelKFOT");
    }

    ptxDomain* ptxEmitRuleStd::stdEmitterData::CreateDomain(uint32_t domainType, ptxDomain::eDomainShape shape)
    {
        switch(shape)
        {
            case ptxDomain::eDomainShape::BOX:
                return new ptxDomainBox(domainType);
            break;

            case ptxDomain::eDomainShape::SPHERE:
                return new ptxDomainSphere(domainType);
            break;

            case ptxDomain::eDomainShape::CYLINDER:
                return new ptxDomainCylinder(domainType);
            break;

            case ptxDomain::eDomainShape::VORTEX:
                return new ptxDomainVortex(domainType);
            break;

            default:
                Log::Error("Invalid ptx domain shape - %d", shape);
               return nullptr;
        }
    }
}