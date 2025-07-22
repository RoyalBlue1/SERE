#pragma once

#include "RuiNodeEditor.h"
#include "RenderManager.h"


class Transform0Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Null Transform";
	static inline std::string category = "Transform";
private:
	RenderInstance& proto;
public:
	explicit Transform0Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class Transform1Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Copy Transform";
	static inline std::string category = "Transform";
private:
	RenderInstance& proto;
public:
	explicit Transform1Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};


class Transform2Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Transform 2";
	static inline std::string category = "Transform";
private:
	RenderInstance& proto;
public:
	explicit Transform2Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class Transform3Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Transform 3";
	static inline std::string category = "Transform";
private:
	RenderInstance& proto;
public:
	explicit Transform3Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class Transform4Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Transform 4";
	static inline std::string category = "Transform";
private:
	RenderInstance& proto;
public:
	explicit Transform4Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class Transform5Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Transform 5";
	static inline std::string category = "Transform";
private:
	RenderInstance& proto;
public:
	explicit Transform5Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class Transform6Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Transform 6";
	static inline std::string category = "Transform";
private:
	RenderInstance& proto;
public:
	explicit Transform6Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class Transform7Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "2 Pin Scale Transform";
	static inline std::string category = "Transform";
private:
	RenderInstance& proto;
public:
	explicit Transform7Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class Transform8Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "2 Pin Pinch Transform";
	static inline std::string category = "Transform";
private:
	RenderInstance& proto;
public:
	explicit Transform8Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class Transform9Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "2 Pin Stretch Transform";
	static inline std::string category = "Transform";
private:
	RenderInstance& proto;
public:
	explicit Transform9Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class Transform10Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "3 Pin Transform";
	static inline std::string category = "Transform";
private:
	RenderInstance& proto;
public:
	explicit Transform10Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class Transform11Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Rotate Transform";
	static inline std::string category = "Transform";
private:
	RenderInstance& proto;
public:
	explicit Transform11Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class Transform12Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Transform 12";
	static inline std::string category = "Transform";
private:
	RenderInstance& proto;
public:
	explicit Transform12Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class Transform13Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Transform 13";
	static inline std::string category = "Transform";
private:
	RenderInstance& proto;
public:
	explicit Transform13Node(RenderInstance& prot);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};


void AddTransformNodes(NodeEditor& editor);