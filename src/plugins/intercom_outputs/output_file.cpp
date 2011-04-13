/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <usbase/model.h>
#include <usbase/parameter.h>
#include <usbase/utilities.h>
#include "../intercom_models/area.h"
#include "../intercom_models/constants.h"
#include "../intercom_models/layer.h"
#include "../intercom_models/light_components.h"
#include "../intercom_models/photosynthetic_rate.h"
#include "../intercom_models/plant.h"
#include "../intercom_models/plant_layers.h"
#include "../intercom_models/time_slice.h"
#include "output_file.h"

using namespace UniSim;

namespace intercom{
	
OutputFile::OutputFile(Identifier name, QObject *parent)
    : Output(name, parent), os(&file)
{
    new Parameter<QString>("fileName", &fileName, QString("output_intercom.prn"), this, "description");
}

void OutputFile::initialize() {
    Output::initialize();
}

void OutputFile::cleanup() {
    openFile();
    seekObjects();
    os << "INTERCOM Details\n\n";

    os << "Daylength (h) " << calendar->pullVariable<double>("dayLength") << "\n\n";

    os << "Irradiation (J/m2/d); PAR [J/s/m2] (total, diffuse, direct)\n";
    for (int i = 0; i < 3; ++i)
        writeWeather(slices[i]);
    os << "\n";

    for (int i = 0; i < 3; ++i)
        writeLayerHeights(slices[i]);

    for (int i = 0; i < 3; ++i)
        writeLayerELAI(slices[i]);

    for (int i = 0; i < 3; ++i)
        writeAbsorption(slices[i]);

    file.close();
}

void OutputFile::openFile() {
    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QString filePath = path + "/" + fileName;
    file.setFileName(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
        throw Exception("Could not open output file to write table:\n'" + filePath + "'");

}

void OutputFile::seekObjects() {
    calendar = seekOne<Model*>("calendar");
    weather = seekOne<Model*>("weather");
    slices = seekMany<TimeSlice*>("*");
    Q_ASSERT(slices.size() == 3);
}

Plant* OutputFile::plant(TimeSlice *slice) {
    return plantLayers(slice)->plant();
}


PlantLayers* OutputFile::plantLayers(TimeSlice *slice) {
    QList<PlantLayers*> plantLayersList = slice->seekChildren<PlantLayers*>("*");
    Q_ASSERT(plantLayersList.size() > 0);
    return plantLayersList[0];
}

QList<Layer*> OutputFile::layers(TimeSlice *slice) {
    QList<Layer*> layers = plantLayers(slice)->seekChildren<Layer*>("*");
    Q_ASSERT(layers.size() == 5);
    return layers;
}

void OutputFile::writeWeather(TimeSlice *slice) {
    slice->tick();

    os << slice->id().label() << ", hour = " << slice->hour() << "\t";

    double irradiation = weather->pullVariable<double>("irradiation");
    double parTotal = weather->pullVariable<double>("parTotal");
    double parDiffuse = weather->pullVariable<double>("parDiffuse");
    double parDirect = weather->pullVariable<double>("parDirect");
    os << "\t" << irradiation
       << "\t" << parTotal
       << "\t" << parDiffuse
       << "\t" << parDirect << "\n";
}

void OutputFile::writeLayerHeights(TimeSlice *slice) {
    Model *plantHeight = plant(slice)->seekOneChild<Model*>("height");
    double height = plantHeight->pullVariable<double>("height");

    os << plant(slice)->id().label() << ", canopy layers, height\n";
    os << slice->id().label() << ", hour = " << slice->hour() << "\n";

    for (int i = 0; i < 5; ++i)
        os << i <<  "\t"  << layers(slice)[i]->height() << "\n";

    os << "top\t" << height << "\n\n";
}

void OutputFile::writeLayerELAI(TimeSlice *slice) {
    slice->tick();

    os << plant(slice)->id().label() <<", canopy layers, ELAI above (Diffuse, DirectDirect, DirectTotal)\n";
    os << slice->id().label() << ", hour = " << slice->hour() << "\n";

    for (int i = 0; i < 5; ++i) {
        LightComponents elai = layers(slice)[i]->ELAI();
        os << i;
        for (int j = 0; j < 3; ++j) {
            os <<  "\t" << elai[j];
        }
        os << "\n";
    }
    os << "\n";
}

void OutputFile::writeAbsorption(TimeSlice *slice) {
    slice->tick();

    QList<Area*> areas = plant(slice)->seekDescendants<Area*>("*");
    for (int i = 0; i < areas.size(); ++i)
        writeAbsorption(slice, areas[i]);
}

void OutputFile::writeAbsorption(TimeSlice *slice, Area *area) {
    os << area->fullName()
       << "; canopy layers; absorption in shade (Diffuse, DirectDirect, DirectTotal, Net), in sun;"
       << "sunlit fraction; area; absorption total\n";

    os << slice->id().label() << ", hour = " << slice->hour() << "\n";

    double sum = 0.;
    for (int i = 0; i < 5; ++i) {
        LightComponents elai = layers(slice)[i]->ELAI();
        LightComponents absorptionInShade = area->calcAbsorptionInShade(elai);
        os << i;
        for (int j = 0; j < 3; ++j) {
            os <<  "\t" << absorptionInShade[j];
        }

        double secondaryDiffuse = absorptionInShade[DirectTotal] - absorptionInShade[DirectDirect];
        if (secondaryDiffuse < 0.)
            secondaryDiffuse = 0.;
        double absorptionInShadeNet = absorptionInShade[Diffuse] + secondaryDiffuse;
        os <<  "\t" << absorptionInShadeNet;

        PhotosyntheticRate photosynthesisInSun = area->calcPhotosynthesisInSun(absorptionInShadeNet);
        double absorptionInSunNet = photosynthesisInSun.absorption();
        os <<  "\t" << absorptionInSunNet;

        double sunlit = exp(-elai.value(DirectDirect));
        os <<  "\t" << sunlit;

        double height = layers(slice)[i]->height();
        double areaDensity = area->atHeight(height);
        os <<  "\t" << areaDensity;

        PhotosyntheticRate photosynthesisInShade = area->calcPhotosynthesisInShade(elai);
        PhotosyntheticRate photosynthesisTotal = area->calcPhotosynthesisTotal(height, elai, photosynthesisInShade, photosynthesisInSun);
        double absorptionTotal =
            photosynthesisTotal.absorption()*WGAUSS5[i]*WGAUSS3[slice->index()]*slice->dayLength();
        os <<  "\t" << absorptionTotal;

        os << "\n";
        sum += absorptionTotal;
    }
    sum *= 3600.;
    os << "Total " << "\t" << sum << "\n\n";

}





} //namespace

