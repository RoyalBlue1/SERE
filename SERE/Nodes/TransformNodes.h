#pragma once

#include "RuiNodeEditor.h"
#include "RenderManager.h"


class Transform0Node : public RuiBaseNode
{
public:
	static inline std::string name = "Null Transform";
	static inline std::string category = "Transform";

	explicit Transform0Node(RenderInstance& prot,NodeStyles& styles);
	explicit Transform0Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class Transform1Node : public RuiBaseNode
{
public:
	static inline std::string name = "Copy Transform";
	static inline std::string category = "Transform";

	explicit Transform1Node(RenderInstance& prot,NodeStyles& styles);
	explicit Transform1Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};


class Transform2Node : public RuiBaseNode
{
public:
	static inline std::string name = "Transform 2";
	static inline std::string category = "Transform";

	explicit Transform2Node(RenderInstance& prot,NodeStyles& styles);
	explicit Transform2Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class Transform3Node : public RuiBaseNode
{
public:
	static inline std::string name = "Transform 3";
	static inline std::string category = "Transform";

	explicit Transform3Node(RenderInstance& prot,NodeStyles& styles);
	explicit Transform3Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class Transform4Node : public RuiBaseNode
{
public:
	static inline std::string name = "Transform 4";
	static inline std::string category = "Transform";

	explicit Transform4Node(RenderInstance& prot,NodeStyles& styles);
	explicit Transform4Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class Transform5Node : public RuiBaseNode
{
public:
	static inline std::string name = "Transform 5";
	static inline std::string category = "Transform";

	explicit Transform5Node(RenderInstance& prot,NodeStyles& styles);
	explicit Transform5Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class Transform6Node : public RuiBaseNode
{
public:
	static inline std::string name = "Transform 6";
	static inline std::string category = "Transform";

	explicit Transform6Node(RenderInstance& prot,NodeStyles& styles);
	explicit Transform6Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class Transform7Node : public RuiBaseNode
{
public:
	static inline std::string name = "2 Pin Scale Transform";
	static inline std::string category = "Transform";

	explicit Transform7Node(RenderInstance& prot,NodeStyles& styles);
	explicit Transform7Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class Transform8Node : public RuiBaseNode
{
public:
	static inline std::string name = "2 Pin Pinch Transform";
	static inline std::string category = "Transform";

	explicit Transform8Node(RenderInstance& prot,NodeStyles& styles);
	explicit Transform8Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class Transform9Node : public RuiBaseNode
{
public:
	static inline std::string name = "2 Pin Stretch Transform";
	static inline std::string category = "Transform";

	explicit Transform9Node(RenderInstance& prot,NodeStyles& styles);
	explicit Transform9Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class Transform10Node : public RuiBaseNode
{
public:
	static inline std::string name = "3 Pin Transform";
	static inline std::string category = "Transform";

	explicit Transform10Node(RenderInstance& prot,NodeStyles& styles);
	explicit Transform10Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class Transform11Node : public RuiBaseNode
{
public:
	static inline std::string name = "Rotate Transform";
	static inline std::string category = "Transform";

	explicit Transform11Node(RenderInstance& prot,NodeStyles& styles);
	explicit Transform11Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class Transform12Node : public RuiBaseNode
{
public:
	static inline std::string name = "Transform 12";
	static inline std::string category = "Transform";

	explicit Transform12Node(RenderInstance& prot,NodeStyles& styles);
	explicit Transform12Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class Transform13Node : public RuiBaseNode
{
public:
	static inline std::string name = "Transform 13";
	static inline std::string category = "Transform";

	explicit Transform13Node(RenderInstance& prot,NodeStyles& styles);
	explicit Transform13Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};


void AddTransformNodes(NodeEditor& editor);