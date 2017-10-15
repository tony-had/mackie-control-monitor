# Mackie Control Monitor

An application written in JUCE that interprets incoming MIDI messages encoded using the Mackie Control protocol as described in Apple's *Logic Pro 7.2.1 - Dedicated Control Surface Support* document.

## Version History

### 0.1
* very ugly - lots of duplication and a lot of code in a single file
* written as a JUCE and Mackie Control introduction exercise 
* the message classification (type/subtype/id, etc.) is made-up
* decodes all incoming messages with (where applicable):
    * channel number
    * LCD text
    * button name
