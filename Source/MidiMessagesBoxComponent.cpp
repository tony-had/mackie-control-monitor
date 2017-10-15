/*
  ==============================================================================

    MidiMessagesBoxComponent.cpp
    Created: 10 Oct 2017 3:39:56pm
    Author:  tonyh

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiMessagesBoxComponent.h"

namespace
{
    enum class ColId : int
    {
        Timestamp = 1,
        Type,
        Subtype,
        ID,
        Value,
        RawData
    };
}

//==============================================================================
MidiMessagesBoxComponent::MidiMessagesBoxComponent():font(14.0f)
{
	addAndMakeVisible(table);
	table.setModel(this);

	table.setColour(ListBox::outlineColourId, Colours::grey);
	table.setOutlineThickness(1);

	table.getHeader().addColumn("Timestamp", int(ColId::Timestamp), 100, 50, -1, TableHeaderComponent::defaultFlags);
	table.getHeader().addColumn("Type",      int(ColId::Type),      50,  50, -1, TableHeaderComponent::defaultFlags);
	table.getHeader().addColumn("Subtype",   int(ColId::Subtype),   100, 50, -1, TableHeaderComponent::defaultFlags);
    table.getHeader().addColumn("ID",        int(ColId::ID),        50,  50, -1, TableHeaderComponent::defaultFlags);
	table.getHeader().addColumn("Value",     int(ColId::Value),     150, 50, -1, TableHeaderComponent::defaultFlags);
    table.getHeader().addColumn("Raw Data",  int(ColId::RawData),   150, 50, -1, TableHeaderComponent::defaultFlags);
	table.getHeader().setStretchToFitActive(true);
}

MidiMessagesBoxComponent::~MidiMessagesBoxComponent()
{
}

void MidiMessagesBoxComponent::paint (Graphics& g)
{
	/*
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("MidiMessagesBoxComponent", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
	*/
}

void MidiMessagesBoxComponent::resized()
{
	table.setBoundsInset(BorderSize<int>(0));
}

int MidiMessagesBoxComponent::getNumRows()
{
	return numRows;
}

void MidiMessagesBoxComponent::incrementNumRows()
{
	numRows++;
	table.updateContent();
	table.scrollToEnsureRowIsOnscreen(numRows);
}


void MidiMessagesBoxComponent::paintRowBackground(Graphics& g,
	int rowNumber,
	int /*width*/, int /*height*/,
	bool rowIsSelected)
{
	//g.fillAll(Colours::lightblue);
}

void MidiMessagesBoxComponent::paintCell(Graphics& g,
	int rowNumber,
	int columnId,
	int width, int height,
	bool /*rowIsSelected*/)
{
	g.setColour(getLookAndFeel().findColour(ListBox::textColourId));
	g.setFont(font);

	//g.drawText("Demo" + String(rowNumber) + String(columnId), 2, 0, width - 4, height, Justification::centredLeft, true);

    g.drawText(midiMessagesBoxContents[(columnId - 1) + (rowNumber * 6)], 2, 0, width - 4, height, Justification::centredLeft, true);
    //g.drawText(midiMessagesBoxContents[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
    //Logger::outputDebugString(midiMessagesBoxContents[(columnId - 1) + (rowNumber * 6)]);

	g.fillRect(width - 1, 0, 1, height);
}