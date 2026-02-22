#include "PtxEvent.h"
#include "PtxEmitRule.h"
#include "PtxRule.h"

namespace rage
{
    void ptxEvent::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mEvoGroup.AddToLayout(layout, depth);

        if(mType == eEventType::EMITTER)
        {
            ((ptxEventEmitter*)this)->AddToLayout(layout, depth);
        }
        else if(mType == eEventType::EFFECT)
        {
            ((ptxEventEffect*)this)->AddToLayout(layout, depth);
        }
        else
        {
            Log::Error("Invalid ptxEvent type - %d", (uint32_t)mType);
        }
    }

    void ptxEvent::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mEvoGroup.SerializePtrs(layout, rsc, depth);

        if(mType == eEventType::EMITTER)
        {
            ((ptxEventEmitter*)this)->SerializePtrs(layout, rsc, depth);
        }
        else if(mType == eEventType::EFFECT)
        {
            ((ptxEventEffect*)this)->SerializePtrs(layout, rsc, depth);
        }
        else
        {
            Log::Error("Invalid ptxEvent type - %d", (uint32_t)mType);
        }
    }

    void ptxEvent::Place(void* that, const datResource& rsc)
    {
        if(mType == eEventType::EMITTER)
        {
            new(that) ptxEventEmitter(rsc);
        }
        else if(mType == eEventType::EFFECT)
        {
            new(that) ptxEventEffect(rsc);
        }
        else
        {
            Log::Error("Invalid ptxEvent type - %d", (uint32_t)mType);
        }
    }

    uint32_t ptxEvent::GetObjectSize() const
    {
        if(mType == eEventType::EMITTER)
        {
            return sizeof(ptxEventEmitter);
        }
        else if(mType == eEventType::EFFECT)
        {
            return sizeof(ptxEventEffect);
        }
        else
        {
            Log::Error("Invalid ptxEvent type - %d", (uint32_t)mType);
        }
    }

    const char* ptxEvent::TypeToString(eEventType type)
    {
        static const char* lut[] {"EMITTER", "EFFECT"};

        if((uint32_t)type > (uint32_t)eEventType::COUNT)
            return "INVALID_TYPE";
        else
            return lut[(uint32_t)type];
    }

    ptxEvent::eEventType ptxEvent::StringToType(const char* str)
    {
        static const char* lut[] {"EMITTER", "EFFECT"};

        if(!str)
            return eEventType::COUNT;

        for(size_t i = 0; i < std::size(lut); i++)
        {
            if(stricmp(str, lut[i]) == 0)
                return (eEventType)i;
        }

        return eEventType::COUNT;
    }

    void ptxEvent::WriteToJsonBase(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.String("Type");
        writer.String(TypeToString(mType));

        writer.String("Index");
        writer.Int(mIndex);

        writer.String("StartRatio");
        writer.Double((double)mStartRatio);

        if(mEvoGroup.Get())
        {
            writer.String("EvoGroup");
            mEvoGroup->WriteToJson(writer);
        }

        writer.String("EndRatio");
        writer.Double((double)mEndRatio);

        writer.String("TriggerCap");
        writer.Int(mTriggerCap);

        writer.String("field_1C");
        writer.Int(field_1C);
    }

    void ptxEvent::LoadFromJsonBase(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        mIndex = object["Index"].GetInt();
        mStartRatio = object["StartRatio"].GetFloat();

        if(object.HasMember("EvoGroup"))
        {
            mEvoGroup = {new ptxEvolutionGroup()};
            JsonHelpers::LoadMemberObject(*mEvoGroup, object, "EvoGroup");
        }

        mEndRatio = object["EndRatio"].GetFloat();
        mTriggerCap = object["TriggerCap"].GetInt();
        field_1C = object["field_1C"].GetInt();
    }


    void ptxEffectOverridables::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);

            writer.String("SizeScale");
            writer.StartArray();
            {
                writer.Double((double)mSizeScale.x);
                writer.Double((double)mSizeScale.y);
                writer.Double((double)mSizeScale.z);
            }
            writer.EndArray();

            writer.String("Duration");
            writer.Double((double)mDuration);

            writer.String("PlaybackRate");
            writer.Double((double)mPlaybackRate);

            writer.String("ColorTint");
            writer.StartArray();
            {
                writer.Uint((uint32_t)mColorTint.Red);
                writer.Uint((uint32_t)mColorTint.Green);
                writer.Uint((uint32_t)mColorTint.Blue);
                writer.Uint((uint32_t)mColorTint.Alpha);
            }
            writer.EndArray();

            writer.String("Zoom");
            writer.Double((double)mZoom);

            writer.String("WhichFields");
            writer.Uint(mWhichFields);

            writer.SetFormatOptions(rapidjson::kFormatDefault);
        }
        writer.EndObject();
    }

    void ptxEffectOverridables::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        mSizeScale.x = object["SizeScale"].GetArray()[0].GetFloat();
        mSizeScale.y = object["SizeScale"].GetArray()[1].GetFloat();
        mSizeScale.z = object["SizeScale"].GetArray()[1].GetFloat();

        mDuration = object["Duration"].GetFloat();
        mPlaybackRate = object["PlaybackRate"].GetFloat();

        auto colorTintArray = object["ColorTint"].GetArray();
        mColorTint.Red   = (uint8_t)colorTintArray[0].GetUint();
        mColorTint.Green = (uint8_t)colorTintArray[1].GetUint();
        mColorTint.Blue  = (uint8_t)colorTintArray[2].GetUint();
        mColorTint.Alpha = (uint8_t)colorTintArray[3].GetUint();

        mZoom = object["Zoom"].GetFloat();
        mWhichFields = object["WhichFields"].GetUint();
    }


    ptxEventEffect::ptxEventEffect(const datResource& rsc) : ptxEvent(rsc), mEffectRule(rsc), mEmitterDomain(rsc)
    {
        if(mEffectName)
            rsc.PointerFixUp(mEffectName);
    }

    void ptxEventEffect::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mEffectRule.AddToLayout(layout, depth);
        mEmitterDomain.AddToLayout(layout, depth);

        if(mEffectName)
            layout.AddObject(mEffectName, RSC5Layout::eBlockType::VIRTUAL, strlen(mEffectName) + 1);
    }

    void ptxEventEffect::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mEffectRule.SerializePtrs(layout, rsc, depth);
        mEmitterDomain.SerializePtrs(layout, rsc, depth);

        if(mEffectName)
            layout.SerializePtr(mEffectName, strlen(mEffectName) + 1);
    }

    void ptxEventEffect::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            if(mEffectName)
            {
                writer.String("EffectName");
                writer.String(mEffectName);
            }

            if(mEffectRule.Get())
            {
                writer.String("EffectRule");
                writer.String(mEffectRule->mName);
            }

            WriteToJsonBase(writer);

            writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
            writer.String("RotationMin");
            writer.StartArray();
            {
                writer.Double((double)mRotationMin.x);
                writer.Double((double)mRotationMin.y);
                writer.Double((double)mRotationMin.z);
            }
            writer.EndArray();
            writer.SetFormatOptions(rapidjson::kFormatDefault);

            writer.String("OverrideMins");
            mOverrideMins.WriteToJson(writer);
            writer.String("OverrideMaxes");
            mOverrideMaxes.WriteToJson(writer);

            writer.String("field_98");
            writer.Uint(field_98);

            if(mEmitterDomain.Get())
            {
                writer.String("EmitterDomain");
                mEmitterDomain->WriteToJson(writer);
            }

            writer.String("field_A4");
            writer.Int(field_A4);
        }
        writer.EndObject();
    }

    static ptxDomain* CreateDomain(uint32_t domainFunction, ptxDomain::eDomainShape type)
    {
        switch(type)
        {
            case ptxDomain::eDomainShape::BOX:
                return new ptxDomainBox(domainFunction);
            break;

            case ptxDomain::eDomainShape::SPHERE:
                return new ptxDomainSphere(domainFunction);
            break;

            case ptxDomain::eDomainShape::CYLINDER:
                return new ptxDomainCylinder(domainFunction);
            break;

            case ptxDomain::eDomainShape::VORTEX:
                return new ptxDomainVortex(domainFunction);
            break;

            default:
                return nullptr;
        }
    }

    void ptxEventEffect::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        LoadFromJsonBase(object);
        
        if(object.HasMember("EffectName"))
            mEffectName = strdup(object["EffectName"].GetString());

        if(object.HasMember("EffectRule"))
            mEffectRule = {(ptxEffectRule*)strdup(object["EffectName"].GetString())};

        mRotationMin.x = object["RotationMin"].GetArray()[0].GetFloat();
        mRotationMin.y = object["RotationMin"].GetArray()[1].GetFloat();
        mRotationMin.z = object["RotationMin"].GetArray()[2].GetFloat();

        JsonHelpers::LoadMemberObject(mOverrideMins, object, "OverrideMins");
        JsonHelpers::LoadMemberObject(mOverrideMaxes, object, "OverrideMaxes");

        field_98 = object["field_98"].GetUint();

        if(object.HasMember("EmitterDomain") && object["EmitterDomain"].IsObject())
        {
            auto domainObj = object["EmitterDomain"].GetObject();
            ptxDomain::eDomainShape shape = ptxDomain::StringToShape(domainObj["Shape"].GetString());
            
            mEmitterDomain = { CreateDomain(0, shape) };
            if(mEmitterDomain.Get())
                mEmitterDomain->LoadFromJson(domainObj);
            else
            {
                Log::Error("ptxEventEffect \"%s\"'s EmitterDomain uses an invalid domain shape - %d", mEffectName ? mEffectName : "null", shape);
            }
        }

        field_A4 = object["field_A4"].GetInt();
    }


    ptxEventEmitter::ptxEventEmitter(const datResource& rsc) : ptxEvent(rsc), mEmitterRule(rsc), mParticleRule(rsc)
    {
        if(mEmitterRuleName)
            rsc.PointerFixUp(mEmitterRuleName);
        if(mParticleRuleName)
            rsc.PointerFixUp(mParticleRuleName);
    }

    void ptxEventEmitter::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mEmitterRule.AddToLayout(layout, depth);
        mParticleRule.AddToLayout(layout, depth);

        if(mEmitterRuleName)
            layout.AddObject(mEmitterRuleName, RSC5Layout::eBlockType::VIRTUAL, strlen(mEmitterRuleName) + 1);
        if(mParticleRuleName)
            layout.AddObject(mParticleRuleName, RSC5Layout::eBlockType::VIRTUAL, strlen(mParticleRuleName) + 1);
    }

    void ptxEventEmitter::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mEmitterRule.SerializePtrs(layout, rsc, depth);
        mParticleRule.SerializePtrs(layout, rsc, depth);

        if(mEmitterRuleName)
            layout.SerializePtr(mEmitterRuleName, strlen(mEmitterRuleName) + 1);
        if(mParticleRuleName)
            layout.SerializePtr(mParticleRuleName, strlen(mParticleRuleName) + 1);
    }

    void ptxEventEmitter::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            WriteToJsonBase(writer);

            if(mEmitterRuleName)
            {
                writer.String("EmitterRuleName");
                writer.String(mEmitterRuleName);
            }
            if(mParticleRuleName)
            {
                writer.String("ParticleRuleName");
                writer.String(mParticleRuleName);
            }

            writer.String("PlaybackRateScalarMin");
            writer.Double((double)mPlaybackRateScalarMin);
            writer.String("PlaybackRateScalarMax");
            writer.Double((double)mPlaybackRateScalarMax);
            writer.String("TimeScalarMin");
            writer.Double((double)mTimeScalarMin);
            writer.String("TimeScalarMax");
            writer.Double((double)mTimeScalarMax);
            writer.String("ZoomScalarMin");
            writer.Double((double)mZoomScalarMin);
            writer.String("ZoomScalarMax");
            writer.Double((double)mZoomScalarMax);

            writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
            writer.String("ColorTintMin");
            writer.StartArray();
            {
                writer.Uint((uint32_t)mColorTintMin.Red);
                writer.Uint((uint32_t)mColorTintMin.Green);
                writer.Uint((uint32_t)mColorTintMin.Blue);
                writer.Uint((uint32_t)mColorTintMin.Alpha);
            }
            writer.EndArray();

            writer.String("ColorTintMax");
            writer.StartArray();
            {
                writer.Uint((uint32_t)mColorTintMax.Red);
                writer.Uint((uint32_t)mColorTintMax.Green);
                writer.Uint((uint32_t)mColorTintMax.Blue);
                writer.Uint((uint32_t)mColorTintMax.Alpha);
            }
            writer.EndArray();
            writer.SetFormatOptions(rapidjson::kFormatDefault);
        }
        writer.EndObject();
    }

    void ptxEventEmitter::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        LoadFromJsonBase(object);

        if(object.HasMember("EmitterRuleName"))
            mEmitterRuleName = strdup(object["EmitterRuleName"].GetString());

        if(object.HasMember("ParticleRuleName"))
            mParticleRuleName = strdup(object["ParticleRuleName"].GetString());

        mPlaybackRateScalarMin = object["PlaybackRateScalarMin"].GetFloat();
        mPlaybackRateScalarMax = object["PlaybackRateScalarMax"].GetFloat();
        mTimeScalarMin = object["TimeScalarMin"].GetFloat();
        mTimeScalarMax = object["TimeScalarMax"].GetFloat();
        mZoomScalarMin = object["ZoomScalarMin"].GetFloat();
        mZoomScalarMax = object["ZoomScalarMax"].GetFloat();

        mColorTintMin.Red   = (uint8_t)object["ColorTintMin"].GetArray()[0].GetUint();
        mColorTintMin.Green = (uint8_t)object["ColorTintMin"].GetArray()[1].GetUint();
        mColorTintMin.Blue  = (uint8_t)object["ColorTintMin"].GetArray()[2].GetUint();
        mColorTintMin.Alpha = (uint8_t)object["ColorTintMin"].GetArray()[3].GetUint();

        mColorTintMax.Red   = (uint8_t)object["ColorTintMax"].GetArray()[0].GetUint();
        mColorTintMax.Green = (uint8_t)object["ColorTintMax"].GetArray()[1].GetUint();
        mColorTintMax.Blue  = (uint8_t)object["ColorTintMax"].GetArray()[2].GetUint();
        mColorTintMax.Alpha = (uint8_t)object["ColorTintMax"].GetArray()[3].GetUint();
    }
}