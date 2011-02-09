#ifndef UNISIM_RECORDS_H
#define UNISIM_RECORDS_H


Records::Records() {
	for (int i= 0; i < 2; ++i)
		variables[i] = new Variables;
	new Parameter<QDate>("firstDate", &firstDate, "1/1/2000");
	new Parameter<QTime>("firstTime", &firstTime, "00:00:00");
}

Records::~Records() {
	for (int i= 0; i < 2; ++i)
		delete variables[i];
}
          
void Records::initialize() {
	calendar = seekOne<Model*>("calendar");
	double timeStep = calendar->pullVariable<double>("timeStep");  // days
}

void Records::reset() {
	time = firstDate
	QTime zero(0,0);
	time.addSecs( zero.secsTo(firstTime) );
	
	openFile();
	readFirstRow();
	updatevariables();
};

void Records::openFile {
    QString filePath = FileLocations::location(FileLocationInfo::Datasets).absolutePath() +
                       "/" + fileName;
    file.setFileName(filePath);

    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk) throw Exception("Cannot open dataset file: " + filePath);
}

void Records::readFirstRow() {
	readLabels();
	for (int i = 0; i < labels.size(); ++i) {
		for (int j = 0; j < 2; ++j) {
			variables[j]->append(new Variable<double>(labels[i], &values[i], this, "Recorded value read from file"));
		}
	}
}
	
void Records::readLabels()() {
	readLine();
	dateColumn = -1;
	timeColumn = -1;
	for (int i = 0; i < lineItems.size(); ++i) {
		Identifer id = Identifier(lineItems[i]);
		labels.append(id);
		if (id.compare("date"))
			dateColumn = i;
		else if (id.compare("time"))
			timeColumn = i;
	}
	hasDate = dateColumn > -1;
	hasTime = timeColumn > -1;
}

void Records::readVariables()() {
	readLine();
	variables[1] = variables[0];
	for (int i = 0; i < lineItems.size(); ++i) {
	
}

void Records::readLine() {
	QString line;
    while (!file.atEnd() && line.isEmpty()) {
        line = QString(file.readLine());
	lineItems = file.atEnd() ? 
				QStringList() : 
				line.split(QRegExp("\\s+"), QString::SkipEmptyParts);	
}

void Records::update() {
	time += timeStep;
	if (TestNum::le(time, time1)) {
		while (!atEnd && TestNum::gt(time, time2) 
			readNextRow();
	}
	updateVariables();
};

#endif