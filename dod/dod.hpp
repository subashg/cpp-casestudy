#include <tuple>

namespace dod {

template<int NumNodes>
class ElementNodeImpl : public std::array<uint32_t, NumNodes>
{
public:

	void ForEachNode(std::function<void(uint32_t)> fn) const{
		for(auto i : *this)
			fn(i);
	}

	uint32_t GetNumNodes() const { return NumNodes; }
};

class ElementDisplayAttribute
{
public:
	bool IsVisible() const { return !!_attributes; }
	uint32_t GetColor() const { return _color; }
private:	
	uint32_t     _markmask = 0;
	uint32_t     _color = 0;
};

class ElementScalar
{
public:
	const Scalar& Get() const { return *_scalar; }
private:
	std::unique_ptr<Scalar> _scalars;
};

class ElementRef
{
public:
	void* Get() const { return ref; }
private:
	void* ref;
}

class Node
{
private:
	float coords[3];
};

template<typename ...Args>
class ElementContainer
{
public:

    template<typename Type>
	void push_back(const Type& type)
	{
		using container_type = std::vector<Type>;
	}
};

using Line      = ElementNodeImpl<2>;
using Triangle  = ElementNodeImpl<3>;
using Quad      = ElementNodeImpl<4>;
using Pentagon  = ElementNodeImpl<5>;
using Hexagon   = ElementNodeImpl<6>;
using Octagon   = ElementNodeImpl<8>;

template<typename TElementNodeImpl>
class ElementWithAttributeFacade
{
public:
    ElementWithAttributeFacade(TElementNodeImpl& impl, const Mesh& mesh, uint32_t current) 
    : _current(current), _mesh(mesh), _impl(impl) {}

	void ForEachNode(std::function<void(uint32_t)> fn) const
	{
		_impl.ForEachNode(fn);
	}

	uint32_t GetColor() const { return _mesh.GetAttribute(_current).GetColor(); }
	bool IsVisible() const { return _mesh.GetAttribute(_current).IsVisible(); }
	Scalar& GetScalar() const { return _mesh.GetScalar(_current).Get(); }
	void* GetReference() const { return _mesh.GetReference(_current).Get(); }

private:
	const Mesh& _mesh;
	uint32_t _current;
	TElementNodeImpl& _impl;
};

class Mesh
{
public:

    template<typename Fn>
	void ForEachElement(Fn fn)
	{
		std::visit([fn](auto& arg){
			fn(arg)
		},  elements);
	}

	template<typename Fn>
	void ForEachElementWithAttribute(Fn fn)
	{
		uint32_t index = 0;
		std::visit([fn](auto& arg){
			ElementWithAttributeFacade facade(arg, mesh, index++);			
			fn(arg)
		},  elements);
	}

	const ElementDisplayAttribute& GetAttribute(uint32_t index) { return _display[index]; }
	const ElementScalarAttribute& GetScalar(uint32_t index) { return _scalar[index]; }
	const ElementRef& GetReference(uint32_t index) { return _references[index]; }

private:
	std::vector<Node> nodes;

	using Container = ElementContainer<Line, Triangle, Quad, Pentagon, Hexagon, Octagon>;
	// DOD
	Container _elements;
	std::vector<ElementDisplayAttribute> _display;
	std::vector<ElementScalarAttribute>  _scalar;
	std::vector<ElementRef> _references;
};

}

