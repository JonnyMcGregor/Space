/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SpacedAudioProcessor::SpacedAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
	parameters(*this, nullptr, Identifier("Reverb"),
		{
			std::make_unique<AudioParameterFloat>("roomSize", "Room Length", 0, 1, 0.5),
			std::make_unique<AudioParameterFloat>("mix", "Mix", 0, 1, 0.5),
            std::make_unique<AudioParameterFloat>("width", "Room Width", 0, 1, 0.5),
			std::make_unique<AudioParameterFloat>("damping", "Damping", 0, 1, 0.5),
            std::make_unique<AudioParameterFloat>("pan", "Pan", 0, 1, 0.5)
		})
#endif
{
	roomSize = parameters.getRawParameterValue("roomSize");
	mix = parameters.getRawParameterValue("mix");
	width = parameters.getRawParameterValue("width");
	damping = parameters.getRawParameterValue("damping");
    pan = parameters.getRawParameterValue("pan");
}

SpacedAudioProcessor::~SpacedAudioProcessor()
{
}

//==============================================================================
const String SpacedAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SpacedAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SpacedAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SpacedAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SpacedAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SpacedAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SpacedAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SpacedAudioProcessor::setCurrentProgram (int index)
{
}

const String SpacedAudioProcessor::getProgramName (int index)
{
    return {};
}

void SpacedAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SpacedAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	reverb.setSampleRate(sampleRate);
	reverb.setParameters(reverbParameters);
    wetBuffer.setSize(getTotalNumInputChannels(), samplesPerBlock);
    dryBuffer.setSize(getTotalNumInputChannels(), samplesPerBlock);
    
    wetBuffer.clear();
    dryBuffer.clear();
}

void SpacedAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SpacedAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SpacedAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    //Some daws provide a different buffersize in prepareToPlay than in buffer.getNumSamples().
    //Therefore, check and setSize() just in case...
    if(wetBuffer.getNumSamples() != buffer.getNumSamples())
    {
        wetBuffer.setSize(totalNumInputChannels, buffer.getNumSamples());
        dryBuffer.setSize(totalNumInputChannels, buffer.getNumSamples());
    }
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
        wetBuffer.clear (i, 0, wetBuffer.getNumSamples());
        dryBuffer.clear (i, 0, wetBuffer.getNumSamples());
    }
    
	reverbParameters.roomSize = *roomSize;
	reverbParameters.width = *width;
	reverbParameters.damping = *damping;
	reverbParameters.dryLevel = 0.0;
	reverbParameters.wetLevel = 1.0;
    reverbParameters.freezeMode = 0.1;
	reverb.setParameters(reverbParameters);
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        wetBuffer.copyFrom(channel, 0, buffer.getReadPointer(channel), buffer.getNumSamples());
        dryBuffer.copyFrom(channel, 0, buffer.getReadPointer(channel), buffer.getNumSamples());
    }
    
    if (totalNumInputChannels == 1)
        reverb.processMono(wetBuffer.getWritePointer(0), wetBuffer.getNumSamples());

    else
        reverb.processStereo(wetBuffer.getWritePointer(0), wetBuffer.getWritePointer(1), wetBuffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            if (channel == 0)
                dryBuffer.setSample(channel, sample, dryBuffer.getSample(channel, sample) * cos(*pan*MathConstants<float>::halfPi));
            else
                dryBuffer.setSample(channel, sample, dryBuffer.getSample(channel, sample) * sin(*pan*MathConstants<float>::halfPi));
            
            buffer.setSample(channel, sample, (dryBuffer.getSample(channel, sample) * (1 - *mix)) + (wetBuffer.getSample(channel, sample) * *mix));
        }
    }
}

//==============================================================================
bool SpacedAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SpacedAudioProcessor::createEditor()
{
    return new SpacedAudioProcessorEditor (*this);
}

//==============================================================================
void SpacedAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void SpacedAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpacedAudioProcessor();
}
