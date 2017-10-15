/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
	: lastInputIndex (0), 
	  startTime (Time::getMillisecondCounterHiRes() * 0.001)
{
	setOpaque(true);

	addAndMakeVisible(midiInputListLabel);
	midiInputListLabel.setText("MIDI Input", dontSendNotification);
	midiInputListLabel.attachToComponent(&midiInputList, true);

	addAndMakeVisible(midiInputList);
	midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");
	const StringArray midiInputs(MidiInput::getDevices());
	midiInputList.addItemList(midiInputs, 1);
	midiInputList.addListener(this);

	addAndMakeVisible(midiMessagesBoxComponent);

	// find the first enabled device and use that by default
	for (int i = 0; i < midiInputs.size(); i++)
	{
		if (deviceManager.isMidiInputEnabled(midiInputs[i]))
		{
			setMidiInput(i);
			break;
		}
	}

	// if no enabled devices were found just use the first one in the list
	if (midiInputList.getSelectedId() == 0)
		setMidiInput(0);

	setSize(600, 400);
}

MainContentComponent::~MainContentComponent()
{
	deviceManager.removeMidiInputCallback(MidiInput::getDevices()[midiInputList.getSelectedItemIndex()], this);
	midiInputList.removeListener(this);
}

void MainContentComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);
}

void MainContentComponent::resized()
{
	Rectangle<int> area(getLocalBounds());
	midiInputList.setBounds(area.removeFromTop(36).removeFromRight(getWidth() - 150).reduced(8));
	midiMessagesBoxComponent.setBounds(area.reduced(8));
}

void MainContentComponent::comboBoxChanged(ComboBox* box)
{
	if (box == &midiInputList)
		setMidiInput(midiInputList.getSelectedItemIndex());
}

void MainContentComponent::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message)
{
	postMessageToList(message, source->getName());
}

StringArray MainContentComponent::getMackieMessageDescription(const MidiMessage& m)
{
    String type = "";
    String subtype ="";
    String id = "";
    String value = "";
    String rawdata = String::toHexString(m.getRawData(), m.getRawDataSize());
    StringArray messageRow;

    char ascii;
    
    if (m.isSysEx())
    {
        type = "Global Control";
        const uint8* sysex = m.getSysExData();
        int sysexSize = m.getSysExDataSize();  
        String serial;
        String challenge;
        String response;
        String lcdText;
        


        switch (sysex[4])
        {
            case 0: 
                subtype = "Device Query";
                break;
            case 1: 
                subtype = "Host Connection Query";

                for (int i = 5; i < 12; i++)
                {
                    serial += String(sysex[i]);
                }
                for (int i = 12; i < 16; i++)
                {
                    challenge += String::toHexString(sysex[i]);
                }
                value = "Serial Number: " + serial + "; Challenge Code" + challenge;

                break;
            case 2: 
                subtype = "Host Connection Reply";

                for (int i = 5; i < 12; i++)
                {
                    serial += String(sysex[i]);
                }
                for (int i = 12; i < 16; i++)
                {
                    response += String::toHexString(sysex[i]);
                }
                value = "Serial Number: " + serial + "; Response Code" + response;

                break;
            case 3: 
                subtype = "Host Connection Confirmation";

                for (int i = 5; i < 12; i++)
                {
                    serial += String(sysex[i]);
                }
                value = "Serial Number: " + serial;

                break;
            case 4: 
                subtype = "Host Connection Error";

                for (int i = 5; i < 12; i++)
                {
                    serial += String(sysex[i]);
                }
                value = "Serial Number: " + serial;

                break;
            case 11: 
                subtype = "LCD Back Light Saver";
                if (sysex[5] == 0)
                    value = "Off";
                else
                    value = "On; Timeout: " + String(sysex[6]) + " min";
                break;
            case 12: 
                subtype = "Touchless Movable Faders";
                if (sysex[5] == 0)
                    value = "Touch On";
                else
                    value = "Touch Off";
                break;
            case 14: 
                subtype = "Fader Touch Sensitivity";
                id = "Ch." + String(sysex[5]);
                value = String(sysex[6]);
                break;
            case 15: 
                subtype = "Go Offline";
                break;
            case 16: 
                subtype = "Time Code/BBT Display";
                //The digits in the Time Code and Assignment displays are written RIGHT - TO - LEFT,
                //which helps to conserve bandwidth.
                for (int i = sysexSize - 2; i > 5; i--)
                {

                    if (sysex[i] < 64)
                    {
                        ascii = (sysex[i] < 32) ? sysex[i] + 64 : sysex[i];
                        value = String(CharPointer_ASCII(&ascii)).removeCharacters(&ascii+1);
                    }
                    else
                    {
                        ascii = (sysex[i] - 64 < 32) ? sysex[i] : sysex[i] - 64;
                        value = String(CharPointer_ASCII(&ascii)).removeCharacters(&ascii+1) + ".";
                    }
                }
                break;
            case 17: 
                subtype = "Assignment 7-Segment Display";
                //The digits in the Time Code and Assignment displays are written RIGHT - TO - LEFT,
                //which helps to conserve bandwidth.
                for (int i = 7; i > 5; i--)
                {
                    if (sysex[i] < 64)
                    {
                        ascii = (sysex[i] < 32) ? sysex[i] + 64 : sysex[i];
                        value = String(CharPointer_ASCII(&ascii)).removeCharacters(&ascii+1);
                    }
                    else
                    {
                        ascii = (sysex[i] - 64 < 32) ? sysex[i] : sysex[i] - 64;
                        value = String(CharPointer_ASCII(&ascii)).removeCharacters(&ascii+1) + ".";
                    }
                }
                break;
            case 18: 
                subtype = "LCD";
                if (sysex[5] >= 0 && sysex[5] < 56)
                    id = "Upper Line";
                else if (sysex[5] >= 56 && sysex[5] < 112)
                    id = "Lower Line";
                for (int i = 6; i < sysexSize-1; i++)
                {
                    ascii = sysex[i];
                    value += String(CharPointer_ASCII (&ascii)).removeCharacters(&ascii+1);
                }
                break;
            case 19: 
                subtype = "Version Request";
                break;
            case 20: 
                subtype = "Version Reply";
                for (int i = 6; i < sysexSize - 1; i++)
                {
                    ascii = sysex[i];
                    value += String(CharPointer_ASCII (&ascii)).removeCharacters(&ascii+1);
                }
                break;
            case 32: 
                subtype = "Channel Meter Mode";
                id = "Ch." + String(sysex[5] + 1);
                value += (sysex[6] >> 2 == 1) ? "Enable " : "Disable ";
                value += "LCD Level Meter; ";
                value += (sysex[6] >> 1 == 1) ? "Enable " : "Disable ";
                value += "Peak Hold Display; ";
                value += (sysex[6] && 1 == 1) ? "Enable " : "Disable ";
                value += "Signal LED";
                break;
            case 33: 
                subtype = "Global LCD Meter Mode";
                value = (sysex[5] == 0) ? "Horizontal" : "Vertical";
                break;
            case 97: 
                subtype = "All Faders to Minimum";
                break;
            case 98: 
                subtype = "All LEDs Off";
                break;
            case 99: 
                subtype = "Reset";        
                break;
        }
    }
    else
    {
        type = "Common Control";
        
        if (m.isPitchWheel())
        {
            subtype = "Fader";

            if (m.getChannel() < 9)
                id = "Ch." + String(m.getChannel());
            else if (m.getChannel() == 9)
                id = "Master";

            value = String(m.getPitchWheelValue());
        }
        else if (m.isNoteOnOrOff())
        {
            subtype = "Switch/LED";

            int noteNum = m.getNoteNumber();
            int noteVel = m.getVelocity();

            if (noteNum >= 0 && noteNum < 8)
                id = "REC/RDY Ch." + String(noteNum + 1);
            else if (noteNum >= 8 && noteNum < 16)
                id = "SOLO Ch." + String(noteNum % 8 + 1);
            else if (noteNum >= 16 && noteNum < 24)
                id = "MUTE Ch." + String(noteNum % 8 + 1);
            else if (noteNum >= 24 && noteNum < 32)
                id = "SELECT Ch." + String(noteNum % 8 + 1);
            else if (noteNum >= 32 && noteNum < 40)
                id = "V-Select Ch." + String(noteNum % 8 + 1);
            else if (noteNum == 40)
                id = "ASSIGNMENT: TRACK";
            else if (noteNum == 41)
                id = "ASSIGNMENT: SEND";
            else if (noteNum == 42)
                id = "ASSIGNMENT: PAN/SURROUND";
            else if (noteNum == 43)
                id = "ASSIGNMENT: PLUG-IN";
            else if (noteNum == 44)
                id = "ASSIGNMENT: EQ";
            else if (noteNum == 45)
                id = "ASSIGNMENT: INSTRUMENT";
            else if (noteNum == 46)
                id = "FADER BANKS: BANK Left";
            else if (noteNum == 47)
                id = "FADER BANKS: BANK Right";
            else if (noteNum == 48)
                id = "FADER BANKS: CHANNEL Left";
            else if (noteNum == 49)
                id = "FADER BANKS: CHANNEL Right";
            else if (noteNum == 50)
                id = "FLIP";
            else if (noteNum == 51)
                id = "GLOBAL VIEW";
            else if (noteNum == 52)
                id = "NAME/VALUE";
            else if (noteNum == 53)
                id = "SMPTE/BEATS";
            else if (noteNum >= 54 && noteNum < 62)
                id = "F" + String(noteNum - 53);
            else if (noteNum == 62)
                id = "GLOBAL VIEW: MIDI TRACKS";
            else if (noteNum == 63)
                id = "GLOBAL VIEW: INPUTS";
            else if (noteNum == 64)
                id = "GLOBAL VIEW: AUDIO TRACKS";
            else if (noteNum == 65)
                id = "GLOBAL VIEW: AUDIO INSTRUMENT";
            else if (noteNum == 66)
                id = "GLOBAL VIEW: AUX";
            else if (noteNum == 67)
                id = "GLOBAL VIEW: BUSSES";
            else if (noteNum == 68)
                id = "GLOBAL VIEW: OUTPUTS";
            else if (noteNum == 69)
                id = "GLOBAL VIEW: USER";
            else if (noteNum == 70)
                id = "SHIFT";
            else if (noteNum == 71)
                id = "OPTION";
            else if (noteNum == 72)
                id = "CONTROL";
            else if (noteNum == 73)
                id = "CMD/ALT";
            else if (noteNum == 74)
                id = "AUTOMATION: READ/OFF";
            else if (noteNum == 75)
                id = "AUTOMATION: WRITE";
            else if (noteNum == 76)
                id = "AUTOMATION: TRIM";
            else if (noteNum == 77)
                id = "AUTOMATION: TOUCH";
            else if (noteNum == 78)
                id = "AUTOMATION: LATCH";
            else if (noteNum == 79)
                id = "GROUP";
            else if (noteNum == 80)
                id = "UTILITIES: SAVE";
            else if (noteNum == 81)
                id = "UTILITIES: UNDO";
            else if (noteNum == 82)
                id = "UTILITIES: CANCEL";
            else if (noteNum == 83)
                id = "UTILITIES: ENTER";
            else if (noteNum == 84)
                id = "MARKER";
            else if (noteNum == 85)
                id = "NUDGE";
            else if (noteNum == 86)
                id = "CYCLE";
            else if (noteNum == 87)
                id = "DROP";
            else if (noteNum == 88)
                id = "REPLACE";
            else if (noteNum == 89)
                id = "CLICK";
            else if (noteNum == 90)
                id = "SOLO";
            else if (noteNum == 91)
                id = "REWIND";
            else if (noteNum == 92)
                id = "FAST FWD";
            else if (noteNum == 93)
                id = "STOP";
            else if (noteNum == 94)
                id = "PLAY";
            else if (noteNum == 95)
                id = "RECORD";
            else if (noteNum == 96)
                id = "Cursor Up";
            else if (noteNum == 97)
                id = "Cursor Down";
            else if (noteNum == 98)
                id = "Cursor Left";
            else if (noteNum == 99)
                id = "Cursor Right";
            else if (noteNum == 100)
                id = "Zoom";
            else if (noteNum == 101)
                id = "Scrub";
            else if (noteNum == 102)
                id = "User Switch A";
            else if (noteNum == 103)
                id = "User Switch B";
            else if (noteNum >= 104 && noteNum < 112)
                id = "Fader Touch Ch." + String(noteNum - 103);
            else if (noteNum == 112)
                id = "Fader Touch Master";
            else if (noteNum == 113)
                id = "SMPTE LED";
            else if (noteNum == 114)
                id = "BEATS LED";
            else if (noteNum == 115)
                id = "RUDE SOLO LIGHT";
            else if (noteNum == 118)
                id = "Relay click";

            if (noteVel == 0)
                value = "Off";
            else if (noteVel == 1)
                value = "Flashing";
            else if (noteVel == 127)
                value = "On";
        }
        else if (m.isController())
        {
            int ccNum = m.getControllerNumber();
            int ccVal = m.getControllerValue();

            if (ccNum >= 16 && ccNum <= 23)
            {
                subtype = "V-Pot";

                id = "Ch." + String(ccNum - 15);                

                if (ccVal >= 0 && ccVal < 63)
                    value = "CW; ";
                else
                    value = "CCW; ";

                value += String(ccVal % 64) + " ticks";

            }                
            else if (ccNum == 46)
            {
                subtype = "External Controller";

                value = String(m.getControllerValue());
            }
            else if (ccNum >= 48 && ccNum <= 55)
            {
                subtype = "V-Pot LED Ring";

                id = "Ch." + String(ccNum - 47);

                if (ccVal >= 0 && ccVal <= 63)
                    value = "Center LED Off; ";
                else
                    value = "Center LED On";

                if (ccVal % 64 >= 0 && ccVal % 64 < 16)
                    value += "Single Dot Mode; ";
                else if (ccVal % 64 >= 16 && ccVal % 64 < 32)
                    value += "Boost/Cut Mode; ";
                else if (ccVal % 64 >= 32 && ccVal % 64 < 48)
                    value += "Wrap Mode; ";
                else
                    value += "Spread Mode; ";

                //this is the Logic standard
                //value += String(ccVal % 56);

                //this is the Live standard
                value += "Position " + String(ccVal % 16);
                
            }
                
            else if (ccNum == 60)
            {
                subtype = "Jog Wheel";

                if (ccVal >= 0 && ccVal < 63)
                    value = "CW; ";
                else
                    value = "CCW; ";

                value += String(ccVal % 64) + " ticks";
            }
            else if (ccNum >= 64 && ccNum < 74)
            {
                subtype = "Time Code/BBT Display";
                id = "Digit " + String(74 - ccNum);
                if (ccVal < 64)
                {
                    ascii = (ccVal < 32) ? ccVal + 64 : ccVal;
                    value = String(CharPointer_ASCII(&ascii)).removeCharacters(&ascii+1);
                }
                else
                {
                    ascii = (ccVal - 64 < 32) ? ccVal : ccVal - 64;
                    value = String(CharPointer_ASCII(&ascii)).removeCharacters(&ascii+1) + ".";
                }
                
            }
            else if (ccNum >= 74 && ccNum < 77)
            {
                subtype = "Assignment 7-Segment Display";
                id = (ccNum == 74) ? "Right " : "Left ";
                id += "Digit";
                if (ccVal < 32)
                    ascii = ccVal + 64;
                else
                    ascii = ccVal;
                value = String(CharPointer_ASCII (&ascii)).removeCharacters(&ascii+1);
            }
            else
                subtype = String(ccNum);
        }
        else if (m.isChannelPressure())
        {
            subtype = "Metering";

            int chPresVal = m.getChannelPressureValue();

            id = "Ch." + String((chPresVal / 16) + 1);

            if (chPresVal % 16 >= 0 && chPresVal % 16 <= 12)
                value = String((chPresVal % 16) * 100 / 12) + "%";
            else if (chPresVal % 16 == 14)
                value = "Set Overload";
            else if (chPresVal % 16 == 15)
                value = "Clear Overload";
        }
    }    

    messageRow.add(type);
    messageRow.add(subtype);
    messageRow.add(id);
    messageRow.add(value);
    messageRow.add(rawdata);

    return messageRow;
}

void MainContentComponent::setMidiInput(int index)
{
	const StringArray list(MidiInput::getDevices());

	deviceManager.removeMidiInputCallback(list[lastInputIndex], this);

	const String newInput(list[index]);

	if (!deviceManager.isMidiInputEnabled(newInput))
		deviceManager.setMidiInputEnabled(newInput, true);

	deviceManager.addMidiInputCallback(newInput, this);
	midiInputList.setSelectedId(index + 1, dontSendNotification);

	lastInputIndex = index;
}

void MainContentComponent::logMessage()
{
	midiMessagesBoxComponent.incrementNumRows();
	//repaint

}

void MainContentComponent::postMessageToList(const MidiMessage& message, const String& source)
{
	(new IncomingMessageCallback (this, message, source))->post();
}

void MainContentComponent::addMessageToList(const MidiMessage& message, const String& source)
{
	const double time = message.getTimeStamp() - startTime;

	const int hours = ((int)(time / 3600.0)) % 24;
	const int minutes = ((int)(time / 60.0)) % 60;
	const int seconds = ((int)time) % 60;
	const int millis = ((int)(time * 1000.0)) % 1000;

	const String timecode(String::formatted("%02d:%02d:%02d.%03d",
											hours,
											minutes,
											seconds,
											millis));

    StringArray midiMessageBoxRow(getMackieMessageDescription(message));
    midiMessageBoxRow.insert(0, timecode);

    midiMessagesBoxComponent.midiMessagesBoxContents.addArray(midiMessageBoxRow, 0);

    logMessage();
}



