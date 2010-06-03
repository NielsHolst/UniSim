
namespace UniSim {

class Variable: public QObject {
	Q_OBJECT
public:
	Variable(Identifier name, QObject *parent)
		: QObject(parent) 
		{ setObjectName(name.key()); }
	Identifier id() const
		{ return _id; }
	QVariant toVariant() = 0;
private:
	Identifier _id;
};

}

class PullVariableBase : public Variable {
	Q_OBJECT
public:
	PullVariableBase(Identifier name, QObject *parent)
		: Variable(name, parent) { }
};

template class <T>
class PullVariable<T> : public PullVariableBase {
	Q_OBJECT
public:
	PullVariable(Identifier name, const T* valuePtr, QObject *parent)
		: PullVariableBase(name, parent), valuePtr(valuePtr_) { }
	QVariant toVariant()
		{ return QVariant(*valuePtr); }
	T value() const
		{ return *valuePtr; }
	const T* valuePtr() const
		{ return valuePtr; }
private:
	const T* valuePtr;
};


class PushVariableBase : public Variable {
	Q_OBJECT
public:
	PushVariableBase(Identifier name, QObject *parent)
		: Variable(name, parent) { }
};

template class <T>
class PushVariable<T> : public PushVariableBase {
	Q_OBJECT
public:
	PushVariable(Identifier name, T* valuePtr_, QObject *parent)
		: PushVariableBase(name, parent), valuePtr(valuePtr_) { }
	QVariant toVariant() 
		{ return QVariant(*valuePtr); }
	void setValue(T value)
		{ *valuePtr = value; }
	T value() const
		{ return *valuePtr; }
	const T* valuePtr() const;
		{ return valuePtr; }
private:
	T* valuePtr;
};

class ParameterBase : public Variable {
	Q_OBJECT
public:
	ParameterBase(Identifier name, QObject *parent)
		: Variable(name, parent) { }
};

template class <T>
class Parameter<T> : public PullVariableBase {
	Q_OBJECT
public:
	Parameter(Identifier name, T* valuePtr, QObject *parent)
		: ParameterBase(name, parent) { }
	QVariant toVariant() 
		{ return QVariant(*valuePtr); }
	void setValue(T value)
		{ *valuePtr = value; }
	T value() const
		{ return *valuePtr; }
	const T* valuePtr() const;
		{ return valuePtr; }
	void setValueFromString(QString valueStr)
		{ QVariant v = QVariant(valueStr);
		  if (v.canConvert<T>()
			*valuePtr = v.toString();
		  else 
			throw Exception("Cannot convert parameter " + Id().label() + 
			                "to " + QVariant(*valuePtr).typeName() + 
							" type from string value: \"" + valueStr + "\"");
private:
	T* valuePtr;
};
