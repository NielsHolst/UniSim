class LightUseInterface {
public:	
	virtual void resetLightUse() = 0;
	virtual void accumulateLightUse() = 0;
	
	QList<double> layerAbsorption() const;
	QList<double> layerAssimilation() const;
	
private:
	// state
	double absorbed, assimilated;
	QList<double> _layerAbsorption;
	QList<double> _layerAssimilation;
};

template <class TChild>
class LightUseComposite : public LightUseInterface {
public:	
	void resetLightUse();
	void accumulateLightUse();
private:
	QList<TChild*> children;
};

children = seekChildren<TChild*>("*", parent())


template <class TParent, class TChild>
class LightUseComposite::resetLightUse() {
	for (int i = 0; i < children.size(); ++i)
		children[i]->resetLightUse();
}
