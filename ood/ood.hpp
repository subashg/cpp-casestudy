
namespace ood {

class ElementNode;
class Scalar;
class Element
{
public:
	Element(const std::unique_ptr<ElementNode>& nodes) : _nodes(n) {}

	uint32_t GetColor() const { return _color; }
	bool IsVisible() const { return !!_attributes; }
	Scalar& GetScalar() const { return *_scalars; }
	void* GetReference() const { return _ref; }

    template<typename Functor>
	void ForEachNode(Functor fn) const
	{
		nodes->ForEachNode([fn](auto& node) { fn(node);});
	}

private:
	uint32_t     _attributes = 0;
	uint32_t     _color = 0;
	void*        _ref = nullptr;
	std::unique_ptr<Scalar>      _scalars;
	std::unique_ptr<ElementNode> _nodes;
};

class ElementNode
{
public:

	virtual uint32_t GetNumNodes() const = 0;

	virtual void ForEachNode(std::function<void(uint32_t)> fn) const = 0;
};

template<int NumNodes>
class ElementNodeImpl : public ElementNode, std::array<uint32_t, NumNodes>
{
public:
	ElementNodeImpl(const std::vector<uint32_t>& nodes) {
		std::copy(nodes.begin(), nodes.end(), begin());
	}

	void ForEachNode(std::function<void(uint32_t)> fn) const override{
		for(auto i : *this)
			fn(i);
	}

	uint32_t GetNumNodes() const { return NumNodes; }
};

using Line      = ElementNodeImpl<2>;
using Triangle  = ElementNodeImpl<3>;
using Quad      = ElementNodeImpl<4>;
using Pentagon  = ElementNodeImpl<5>;
using Hexagon   = ElementNodeImpl<6>;
using Octagon   = ElementNodeImpl<8>;

class Node
{
private:
	float _coords[3];
};

class Mesh
{
public:

	uint32_t AddNodes(const Node& node)
	{
		_nodes.push_back(node);
	}

	uint32_t AddElement(const std::vector<uint32_t>& nodes)
	{
		_elements.push_back(Element(_CreateNode(nodes)));
		return _elements.size() - 1;
	}

    template<typename Functor>
	void ForEachElement(Functor fn) const
	{
		for(auto e : _elements)
			fn(e);
	}

private:
	std::unique_ptr<ElementNode> _CreateNode(const std::vector<uint32_t>& nodes) const 
	{		
		switch(nodes.size())
		{
		case 2:
		   return std::make_unique<Line>(nodes);
		case 3:
		   return std::make_unique<Triangle>(nodes);
		case 4:
		   return std::make_unique<Quad>(nodes);
		case 5:
		   return std::make_unique<Pentagon>(nodes);
		case 6:
		   return std::make_unique<Hexagon>(nodes);
		case 8:
		   return std::make_unique<Octagon>(nodes);
		}
	}
private:
	std::vector<Node> _nodes;
	std::vector<Element> _elements;
};

}