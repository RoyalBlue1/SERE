#pragma once

#include "RuiNodeEditor/RuiNodeEditor.h"


class TimeNode : public RuiBaseNode
{
public:
	static inline std::string name = "Current Time";
	static inline std::string category = "Globals";

	explicit TimeNode(RenderInstance& prot,ImFlow::StyleManager& styles);
	explicit TimeNode(RenderInstance& prot,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class ADSFracNode : public RuiBaseNode
{
public:
	static inline std::string name = "ADS Fraction";
	static inline std::string category = "Globals";

	explicit ADSFracNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit ADSFracNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};


class LocalPlayerPosNode : public RuiBaseNode
{
public:
	static inline std::string name = "Local Player Position";
	static inline std::string category = "Globals";

	explicit LocalPlayerPosNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit LocalPlayerPosNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	float minVal;
	float maxVal;
};

class ScreenWidthNode : public RuiBaseNode
{
public:
	static inline std::string name = "Screen Width";
	static inline std::string category = "Globals";

	explicit ScreenWidthNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit ScreenWidthNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	float minVal;
	float maxVal;
};

class ScreenHeightNode : public RuiBaseNode
{
public:
	static inline std::string name = "Screen Height";
	static inline std::string category = "Globals";

	explicit ScreenHeightNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit ScreenHeightNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	float minVal;
	float maxVal;
};


class RandomFloatNode : public RuiBaseNode
{
public:
	static inline std::string name = "Random Float";
	static inline std::string category = "Globals";
	explicit RandomFloatNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit RandomFloatNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	float randomFloat;
};

class BoolGlobalNode : public RuiBaseNode
{
public:
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();

protected:
	BoolGlobalNode(const std::string& nodeName, int Globals::* localField, const std::string& exportField,
		RenderInstance& prot, ImFlow::StyleManager& styles);

private:
	std::string nodeName;
	std::string exportField;
	int Globals::* localField;
};

class IsKillReplayNode : public BoolGlobalNode
{
public:
	static inline std::string name = "Is Kill Replay";
	static inline std::string category = "Globals";

	explicit IsKillReplayNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit IsKillReplayNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
};

class IsUsingControllerNode : public BoolGlobalNode
{
public:
	static inline std::string name = "Is Using Controller";
	static inline std::string category = "Globals";

	explicit IsUsingControllerNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit IsUsingControllerNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
};

class IsAliveNode : public BoolGlobalNode
{
public:
	static inline std::string name = "Is Alive";
	static inline std::string category = "Globals";

	explicit IsAliveNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit IsAliveNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
};

class IsSpectatingNode : public BoolGlobalNode
{
public:
	static inline std::string name = "Is Spectating";
	static inline std::string category = "Globals";

	explicit IsSpectatingNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit IsSpectatingNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
};

class IsMenuOpenNode : public BoolGlobalNode
{
public:
	static inline std::string name = "Is Menu Open";
	static inline std::string category = "Globals";

	explicit IsMenuOpenNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit IsMenuOpenNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
};

class IsPhaseShiftedNode : public BoolGlobalNode
{
public:
	static inline std::string name = "Is Phase Shifted";
	static inline std::string category = "Globals";

	explicit IsPhaseShiftedNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit IsPhaseShiftedNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
};



class ColorGlobalNode : public RuiBaseNode
{
public:
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();

protected:
	ColorGlobalNode(const std::string& nodeName, float (Globals::* localField)[3], const std::string& exportField,
		RenderInstance& prot, ImFlow::StyleManager& styles);

private:
	std::string nodeName;
	std::string exportField;
	float (Globals::* localField)[3];
};

class FriendlyTeamColorNode : public ColorGlobalNode
{
public:
	static inline std::string name = "Friendly Team Color";
	static inline std::string category = "Globals";
	explicit FriendlyTeamColorNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit FriendlyTeamColorNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
};

class EnemyTeamColorNode : public ColorGlobalNode
{
public:
	static inline std::string name = "Enemy Team Color";
	static inline std::string category = "Globals";
	explicit EnemyTeamColorNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit EnemyTeamColorNode(RenderInstance& prot, ImFlow::StyleManager& styles,
		rapidjson::GenericObject<false, rapidjson::Value> obj);
};

class PartyTeamColorNode : public ColorGlobalNode
{
public:
	static inline std::string name = "Party Team Color";
	static inline std::string category = "Globals";
	explicit PartyTeamColorNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit PartyTeamColorNode(RenderInstance& prot, ImFlow::StyleManager& styles,
		rapidjson::GenericObject<false, rapidjson::Value> obj);
};

void AddGlobalNodes(NodeEditor& editor);
