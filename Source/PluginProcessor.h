/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Node.h"
//==============================================================================
/**
*/
class SpacedAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SpacedAudioProcessor();
    ~SpacedAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	AudioProcessorValueTreeState parameters;
    
    bool isFirstTimeOpeningEditor = true;

    Rectangle<float> nodeField;
    Node reverbNode = Node(0, 0, 30, Colour::fromRGB(60, 176, 247), Colour::fromRGB(33,33,33));
	std::vector<Node> roomSizeNodes;
    
private:

	Reverb reverb;

	Reverb::Parameters reverbParameters;
    
    AudioBuffer<float> wetBuffer;
    AudioBuffer<float> dryBuffer;


	float *roomSize = 0;     /**< Room size, 0 to 1.0, where 1.0 is big, 0 is small. */
	float *damping = 0;     /**< Damping, 0 to 1.0, where 0 is not damped, 1.0 is fully damped. */
	float *mix = 0;     /**< Dry level, 0 to 1.0 */
	float *width = 0;
    float *pan = 0;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpacedAudioProcessor)
};
