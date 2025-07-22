#pragma once

#include "RuiNodeEditor.h"


class SplitFloat2Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Split Vector2";
	static inline std::string category = "Split Merge";
private:
	RenderInstance& proto;
public:
	explicit SplitFloat2Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};


class MergeFloat2Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Merge Vector2";
	static inline std::string category = "Split Merge";
private:
	RenderInstance& proto;
public:
	explicit MergeFloat2Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class SplitFloat3Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Split Vector3";
	static inline std::string category = "Split Merge";
private:
	RenderInstance& proto;
public:
	explicit SplitFloat3Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};


class MergeFloat3Node : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Merge Vector3";
	static inline std::string category = "Split Merge";
private:
	RenderInstance& proto;
public:
	explicit MergeFloat3Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class SplitColorNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Split Color RGB";
	static inline std::string category = "Split Merge";
private:
	RenderInstance& proto;
public:
	explicit SplitColorNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};


class RGBToColorNode : public ImFlow::BaseNode
{public:
	static inline std::string name = "Merge Color RGB";
	static inline std::string category = "Split Merge";
private:
	RenderInstance& proto;
public:
	explicit RGBToColorNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};


class HSVToColorNode : public ImFlow::BaseNode
{public:
	static inline std::string name = "Merge Color HSV";
	static inline std::string category = "Split Merge";
private:
	RenderInstance& proto;
public:
	explicit HSVToColorNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

void AddSplitMergeNodes(NodeEditor& editor);