#pragma once


#include <map>
#include <string>

class IInfoNode {
public:
	virtual ~IInfoNode() = default;
	virtual std::string to_string() = 0;
};

template<typename T>
class InfoNode : public IInfoNode {
public:
	std::string _property_name;
	T _property_value;

	InfoNode(){}
	InfoNode(const std::string& _prop, T _val) : _property_name(_prop),_property_value(_val) {
		
	}

	std::string to_string() override;
	~InfoNode() override = default;
};

template <>
inline std::string InfoNode<int>::to_string() {
	return _property_name + " : " + std::to_string(_property_value);
}


class InstanceInfo {
public:
	std::map<std::string, IInfoNode*> _content{};

	template<typename T>
	void AddInfo(std::string _str, T _elem) {
		if(_content.contains(_str)) {
			delete _content.at(_str);
			_content.erase(_str);
		}
		_content.insert(_str, new InfoNode<T>(_str,_elem));
	}
	size_t count_elem() const { return _content.size(); }
};

inline std::ostream& operator<<(std::ostream& out, const InstanceInfo& info) {
	for(const std::pair<const std::string,IInfoNode*>& node : info._content) {
		out << node.second->to_string() << "\n";
	}
	return out;
}


