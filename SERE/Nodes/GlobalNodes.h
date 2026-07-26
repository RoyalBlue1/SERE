#pragma once

#include "RuiNodeEditor/RuiNodeEditor.h"


class TimeNode : public RuiBaseNode
{
public:
	static inline std::string name = "Current Time";
	static inline std::string category = "Globals";

	explicit TimeNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit TimeNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class ADSFracNode : public RuiBaseNode
{
public:
	static inline std::string name = "ADS Fraction";
	static inline std::string category = "Globals";

	explicit ADSFracNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit ADSFracNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
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

	explicit LocalPlayerPosNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit LocalPlayerPosNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
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

	explicit ScreenWidthNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit ScreenWidthNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
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

	explicit ScreenHeightNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit ScreenHeightNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	float minVal;
	float maxVal;
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
		const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);

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

	explicit IsKillReplayNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit IsKillReplayNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
};

class IsUsingControllerNode : public BoolGlobalNode
{
public:
	static inline std::string name = "Is Using Controller";
	static inline std::string category = "Globals";

	explicit IsUsingControllerNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit IsUsingControllerNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
};

class IsAliveNode : public BoolGlobalNode
{
public:
	static inline std::string name = "Is Alive";
	static inline std::string category = "Globals";

	explicit IsAliveNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit IsAliveNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
};

class IsSpectatingNode : public BoolGlobalNode
{
public:
	static inline std::string name = "Is Spectating";
	static inline std::string category = "Globals";

	explicit IsSpectatingNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit IsSpectatingNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
};

class IsMenuOpenNode : public BoolGlobalNode
{
public:
	static inline std::string name = "Is Menu Open";
	static inline std::string category = "Globals";

	explicit IsMenuOpenNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit IsMenuOpenNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
};

class IsPhaseShiftedNode : public BoolGlobalNode
{
public:
	static inline std::string name = "Is Phase Shifted";
	static inline std::string category = "Globals";

	explicit IsPhaseShiftedNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit IsPhaseShiftedNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
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
		const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);

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
	explicit FriendlyTeamColorNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit FriendlyTeamColorNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
};

class EnemyTeamColorNode : public ColorGlobalNode
{
public:
	static inline std::string name = "Enemy Team Color";
	static inline std::string category = "Globals";
	explicit EnemyTeamColorNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit EnemyTeamColorNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles,
		rapidjson::GenericObject<false, rapidjson::Value> obj);
};

class PartyTeamColorNode : public ColorGlobalNode
{
public:
	static inline std::string name = "Party Team Color";
	static inline std::string category = "Globals";
	explicit PartyTeamColorNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit PartyTeamColorNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles,
		rapidjson::GenericObject<false, rapidjson::Value> obj);
};

void AddGlobalNodes(const std::unique_ptr<NodeEditor>& editor);