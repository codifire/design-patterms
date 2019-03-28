#pragma once
#include <string>
#include <vector>
#include <algorithm>

namespace flyweight_pattern
{
	enum class SoldierTypes
	{
		Normal,
		ParaSF,
		Marcos,
		NavySeal,
		GreenBeret,
		Marine,
		Sniper
	};

	enum class SoldierTexture_Uniform
	{
		Green_Camouflage,
		Green_DarkGreenStripesCamo,
		Green_BlackStripesCamo,
		Black,
		DarkBlue_LightBlueStripesCamo,
	};

	enum class SoldierTexture_Gloves
	{
		Black,
		DarkGrey,
		BlackAndGrey,
		Olive
	};

	enum class SoldierTexture_Glasses
	{
		None,
		AviatorGolden,
		AviatorSilver,
		Wayfarer,
		Sporty
	};

	enum class SoldierTexture_Helmet
	{
		BlackSF,
		OliveSF,
		Green
	};

	enum class SoldierTexture_Boots
	{
		Black,
		BlackLong
	};

	// intrinsic state
	class Mesh{};
	class Texture{};
	class Animation{};

	// extrinsic state
	class Position{};
	class Orientation{};
	class Color{};

	// different for different soldiers
	struct ExtrinsicState_Soldier
	{
		Position position;
		Orientation rotation;
		Color color;
		std::string name;
		int64_t id;
	};

	struct NPCSoldierBase_Flyweight
	{
		virtual bool Load() = 0;
		virtual void Unload() = 0;
		virtual void Draw(const ExtrinsicState_Soldier& _drawParams) const = 0;
	};

	struct SoldierPolicy
	{
		SoldierTypes type;
		SoldierTexture_Uniform uniform;
		SoldierTexture_Gloves gloves;
		SoldierTexture_Glasses glasses;
		SoldierTexture_Helmet helmet;
		SoldierTexture_Boots boots;
	};

	//Policy Flyweights
	const SoldierPolicy ParaSF { SoldierTypes::ParaSF, SoldierTexture_Uniform::Green_DarkGreenStripesCamo, SoldierTexture_Gloves::Olive, SoldierTexture_Glasses::None, SoldierTexture_Helmet::OliveSF, SoldierTexture_Boots::BlackLong };
	const SoldierPolicy Normal { SoldierTypes::Normal, SoldierTexture_Uniform::Green_Camouflage, SoldierTexture_Gloves::Olive, SoldierTexture_Glasses::None, SoldierTexture_Helmet::Green, SoldierTexture_Boots::Black };
	const SoldierPolicy Marcos { SoldierTypes::Marcos, SoldierTexture_Uniform::Black, SoldierTexture_Gloves::Black, SoldierTexture_Glasses::None, SoldierTexture_Helmet::BlackSF, SoldierTexture_Boots::BlackLong };
	const SoldierPolicy NvSeal { SoldierTypes::NavySeal, SoldierTexture_Uniform::Green_BlackStripesCamo, SoldierTexture_Gloves::Black, SoldierTexture_Glasses::None, SoldierTexture_Helmet::BlackSF, SoldierTexture_Boots::BlackLong };

	// shared flyweight
	struct NPCSoldierFlyweightConcrete : public NPCSoldierBase_Flyweight
	{
		Mesh meshBody, meshGloves, meshBoots, meshGlasses, meshUniform;
		Texture uniform;
		Texture helmet;
		Texture gloves;
		Texture boots;
		Texture glasses;

		Animation animations[10];
		
		const SoldierPolicy* policy;

		NPCSoldierFlyweightConcrete(const SoldierPolicy* _pol): policy(_pol) {}
		
		void Unload() override {/*Unload Policy*/ };
		bool Load() override {/*Load Policy*/}
		void Draw(const ExtrinsicState_Soldier& _drawParams) const override {}
	};

	struct NPCSoldierFlyweightFactory
	{
	public:
		NPCSoldierFlyweightConcrete* create(const SoldierPolicy* _policy)
		{
			auto result = std::find_if(sharedFlyweights.begin(), sharedFlyweights.end(), [_policy] (const NPCSoldierFlyweightConcrete* _soldier)
			{
				return _soldier->policy == _policy;
			});

			NPCSoldierFlyweightConcrete* found = nullptr;

			if (result == sharedFlyweights.end())
			{
				found = new NPCSoldierFlyweightConcrete(_policy);
			}
			else
			{
				found = *result;
			}

			return found;
		}
	private:
		std::vector<NPCSoldierFlyweightConcrete*> sharedFlyweights;
	};

	NPCSoldierFlyweightFactory factory;

	struct NPCSoldierUnsharedFlyweight: public NPCSoldierBase_Flyweight
	{
		ExtrinsicState_Soldier state;
		NPCSoldierFlyweightConcrete* flyweight;

		NPCSoldierUnsharedFlyweight(const ExtrinsicState_Soldier& _initState, NPCSoldierFlyweightConcrete* _initFlyweight)
			: state(_initState)
			, flyweight(_initFlyweight)
		{}

		void Draw(const ExtrinsicState_Soldier& _drawParams) const override
		{
			flyweight->Draw(_drawParams);
		}

		bool Load() override
		{
			flyweight->Load();
		}

		void Unload() override
		{
			flyweight->Unload();
		}

		void Draw()
		{
			Draw(this->state);
		}
	};

	std::vector<NPCSoldierUnsharedFlyweight*> soldiers;

	NPCSoldierUnsharedFlyweight* AddSoldiers(const ExtrinsicState_Soldier& state, const SoldierPolicy* _policy)
	{
		soldiers.push_back(new NPCSoldierUnsharedFlyweight(state, factory.create(_policy)));
	}
}
