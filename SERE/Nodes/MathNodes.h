#pragma once

#include "RuiNodeEditor/RuiNodeEditor.h"
#include "CustomImGuiWidgets.h"

enum class MathNodeConnectionType
{
	None,
	Float,
	Float2,
	Float3,
	Color,
	Size,
	Invalid
};

class BaseMathNode :public RuiBaseNode
{
public:
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;

protected:
	explicit BaseMathNode(const std::string& name,const std::string& category,std::vector<std::shared_ptr<ImFlow::PinProto>> pinInfo,RenderInstance& prot,ImFlow::StyleManager& styles);

	void UpdateInPin(const char* name,MathNodeConnectionType& lastConnectionType,std::unordered_map<std::string,std::any>& emptyVals);
	void UpdateOutPinVisibility();
	virtual MathNodeConnectionType GetConnectionRestrictions() = 0;
	MathNodeConnectionType OutConnectionType();

private:
	std::string nodeName;
	std::string nodeCategory;
};


class UnaryMathNode : public BaseMathNode
{
protected:
	explicit UnaryMathNode(const std::string& name,const std::string& category,RenderInstance& prot,ImFlow::StyleManager& styles);

	virtual float Operation(float val) = 0;
	virtual std::string OperationString(std::string val) = 0;

	MathNodeConnectionType GetConnectionRestrictions() override;

	std::unordered_map<std::string,std::any> inPinEmptyVal;
	MathNodeConnectionType lastConnectionType;
public:

	void draw() override;
	void Export(RuiExportPrototype& proto) override;
	bool CanCreateLink(ImFlow::Pin* own,ImFlow::Pin* other) override;
	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();



};

class BinaryMathNode : public BaseMathNode
{
protected:
	explicit BinaryMathNode(const std::string& name,const std::string& category,RenderInstance& prot,ImFlow::StyleManager& styles);

	virtual float Operation(float a,float b) = 0;
	virtual std::string OperationString(std::string a,std::string b) = 0;

	MathNodeConnectionType GetConnectionRestrictions() override;

	std::unordered_map<std::string,std::any> inPinEmptyVal;
	MathNodeConnectionType lastConnectionType;
public:

	void draw() override;
	void Export(RuiExportPrototype& proto) override;
	bool CanCreateLink(ImFlow::Pin* own,ImFlow::Pin* other) override;
	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();


};


void AddMathNodes(NodeEditor& editor);

class MultiplyNode : public BinaryMathNode
{
public:
	static inline std::string name = "Multiply";
	static inline std::string category = "Math";

	

	explicit MultiplyNode(RenderInstance& prot,ImFlow::StyleManager& styles);
	explicit MultiplyNode(RenderInstance& prot,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
protected:
	float Operation(float a,float b) override;
	std::string OperationString(std::string a,std::string b) override;
};

class AdditionNode : public BinaryMathNode
{
public:
	static inline std::string name = "Add";
	static inline std::string category = "Math";

	explicit AdditionNode(RenderInstance& prot,ImFlow::StyleManager& styles);
	explicit AdditionNode(RenderInstance& prot,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);

protected:
	float Operation(float a,float b) override;
	std::string OperationString(std::string a,std::string b) override;
};


class SubtractNode : public BinaryMathNode
{
public:
	static inline std::string name = "Subtract";
	static inline std::string category = "Math";
private:
	
public:
	explicit SubtractNode(RenderInstance& prot,ImFlow::StyleManager& styles);
	explicit SubtractNode(RenderInstance& prot,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
protected:
	float Operation(float a,float b) override;
	std::string OperationString(std::string a,std::string b) override;
};

class DivideNode : public BinaryMathNode
{public:
	static inline std::string name = "Divide";
	static inline std::string category = "Math";
private:
	
	std::shared_ptr<ImFlow::NodeStyle> style;
	std::shared_ptr<ImFlow::NodeStyle> errorStyle;
public:
	explicit DivideNode(RenderInstance& prot,ImFlow::StyleManager& style);
	explicit DivideNode(RenderInstance& prot,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj);
protected:
	float Operation(float a,float b) override;
	std::string OperationString(std::string a,std::string b) override;
};

class ModuloNode : public BinaryMathNode
{
public:
	static inline std::string name = "Modulo";
	static inline std::string category = "Math";
private:
	
	std::shared_ptr<ImFlow::NodeStyle> style;
	std::shared_ptr<ImFlow::NodeStyle> errorStyle;
public:
	explicit ModuloNode(RenderInstance& prot,ImFlow::StyleManager& style);
	explicit ModuloNode(RenderInstance& prot,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj);
protected:
	float Operation(float a,float b) override;
	std::string OperationString(std::string a,std::string b) override;
};

class AbsoluteNode : public UnaryMathNode
{
public:
	static inline std::string name = "Absolute";
	static inline std::string category = "Math";
protected:
	float Operation(float val) override;
	std::string OperationString(std::string val) override;

public:
	explicit AbsoluteNode(RenderInstance& prot,ImFlow::StyleManager& style);
	explicit AbsoluteNode(RenderInstance& prot,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj);

};

class SineNode : public UnaryMathNode
{public:
	static inline std::string name = "Sine";
	static inline std::string category = "Math";
protected:
	float Operation(float val) override;
	std::string OperationString(std::string val) override;
	

public:
	explicit SineNode(RenderInstance& prot,ImFlow::StyleManager& style);
	explicit SineNode(RenderInstance& prot,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj);

};

class ExponentNode : public BinaryMathNode
{
public:
	static inline std::string name = "Exponent";
	static inline std::string category = "Math";
protected:
	float Operation(float a,float b) override;
	std::string OperationString(std::string a,std::string b) override;
	

public:
	explicit ExponentNode(RenderInstance& prot,ImFlow::StyleManager& style);
	explicit ExponentNode(RenderInstance& prot,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj);


};

class MappingNode : public RuiBaseNode
{
public:
	static inline std::string name = "Mapping";
	static inline std::string category = "Math";
private:
	
	Mapping map;
public:
	explicit MappingNode(RenderInstance& prot,ImFlow::StyleManager& style);
	explicit MappingNode(RenderInstance& prot,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class TangentNode : public UnaryMathNode
{
public:
	static inline std::string name = "Tangent";
	static inline std::string category = "Math";
protected:
	float Operation(float val) override;
	std::string OperationString(std::string val) override;


public:
	explicit TangentNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit TangentNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);

};

class CosineNode : public UnaryMathNode
{
public:
	static inline std::string name = "Cosine";
	static inline std::string category = "Math";
protected:
	float Operation(float val) override;
	std::string OperationString(std::string val) override;


public:
	explicit CosineNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit CosineNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);

};

class SquareRootNode : public UnaryMathNode
{
public:
	static inline std::string name = "Square root";
	static inline std::string category = "Math";
protected:
	float Operation(float val) override;
	std::string OperationString(std::string val) override;


public:
	explicit SquareRootNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit SquareRootNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);

};

class RoundNode : public UnaryMathNode
{
public:
	static inline std::string name = "Round";
	static inline std::string category = "Math";
protected:
	float Operation(float val) override;
	std::string OperationString(std::string val) override;


public:
	explicit RoundNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit RoundNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);

};

class FloorNode : public UnaryMathNode
{
public:
	static inline std::string name = "Floor";
	static inline std::string category = "Math";
protected:
	float Operation(float val) override;
	std::string OperationString(std::string val) override;


public:
	explicit FloorNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit FloorNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);

};

class CeilNode : public UnaryMathNode
{
public:
	static inline std::string name = "Ceil";
	static inline std::string category = "Math";
protected:
	float Operation(float val) override;
	std::string OperationString(std::string val) override;


public:
	explicit CeilNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit CeilNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);

};

class TruncNode : public UnaryMathNode
{
public:
	static inline std::string name = "Truncate";
	static inline std::string category = "Math";
protected:
	float Operation(float val) override;
	std::string OperationString(std::string val) override;


public:
	explicit TruncNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit TruncNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);

};

class ClampNode : public RuiBaseNode
{
public:
	static inline std::string name = "Clamp";
	static inline std::string category = "Math";
	
	explicit ClampNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit ClampNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;
	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class MinNode : public RuiBaseNode
{
public:
	static inline std::string name = "Min";
	static inline std::string category = "Math";
private:

public:
	explicit MinNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit MinNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;
	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class MaxNode : public RuiBaseNode
{
public:
	static inline std::string name = "Max";
	static inline std::string category = "Math";
	explicit MaxNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit MaxNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;
	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};