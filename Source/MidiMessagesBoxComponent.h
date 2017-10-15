/*
  ==============================================================================

    MidiMessagesBoxComponent.h
    Created: 10 Oct 2017 3:39:56pm
    Author:  tonyh

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class MidiMessagesBoxComponent    : public Component,
									public TableListBoxModel
{
public:
    MidiMessagesBoxComponent();
    ~MidiMessagesBoxComponent();

    void paint (Graphics&) override;
    void resized() override;

	void incrementNumRows();

    StringArray midiMessagesBoxContents;
private:
	TableListBox table;
	Font font;

	int numRows = 0;

	int getNumRows() override;
	
	void paintRowBackground(Graphics&,
							int rowNumber,
							int /*width*/, int /*height*/,
							bool rowIsSelected) override;
	void paintCell(Graphics&,
					int rowNumber,
					int columnId,
					int width, int height,
					bool /*rowIsSelected*/) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiMessagesBoxComponent)
};
