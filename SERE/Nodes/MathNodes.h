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
	explicit BaseMathNode(const std::string& name,const std::string& category,std::vector<std::shared_ptr<ImFlow::PinProto>> pinInfo,const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);

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
	explicit UnaryMathNode(const std::string& name,const std::string& category,const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);

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
	explicit BinaryMathNode(const std::string& name,const std::string& category,const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);

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


void AddMathNodes(const std::unique_ptr<NodeEditor>& editor);

class MultiplyNode : public BinaryMathNode
{
public:
	static inline std::string name = "Multiply";
	static inline std::string category = "Math";

	

	explicit MultiplyNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit MultiplyNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
protected:
	float Operation(float a,float b) override;
	std::string OperationString(std::string a,std::string b) override;
};

class AdditionNode : public BinaryMathNode
{
public:
	static inline std::string name = "Add";
	static inline std::string category = "Math";

	explicit AdditionNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit AdditionNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);

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
	explicit SubtractNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit SubtractNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
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
	explicit DivideNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style);
	explicit DivideNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj);
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
	explicit ModuloNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style);
	explicit ModuloNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj);
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
	explicit AbsoluteNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style);
	explicit AbsoluteNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj);

};

class SineNode : public UnaryMathNode
{public:
	static inline std::string name = "Sine";
	static inline std::string category = "Math";
protected:
	float Operation(float val) override;
	std::string OperationString(std::string val) override;
	

public:
	explicit SineNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style);
	explicit SineNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj);

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
	explicit ExponentNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style);
	explicit ExponentNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj);


};

class MappingNode : public RuiBaseNode
{
public:
	static inline std::string name = "Mapping";
	static inline std::string category = "Math";
private:
	
	Mapping map;
public:
	explicit MappingNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style);
	explicit MappingNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj);
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
	explicit TangentNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit TangentNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);

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
	explicit CosineNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit CosineNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);

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
	explicit SquareRootNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit SquareRootNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);

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
	explicit RoundNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit RoundNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);

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
	explicit FloorNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit FloorNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);

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
	explicit CeilNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit CeilNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);

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
	explicit TruncNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit TruncNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);

};

class ClampNode : public RuiBaseNode
{
public:
	static inline std::string name = "Clamp";
	static inline std::string category = "Math";
	
	explicit ClampNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit ClampNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
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
	explicit MinNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit MinNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
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
	explicit MaxNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit MaxNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;
	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};