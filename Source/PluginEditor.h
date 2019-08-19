/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Node.h"
#include <vector>
//==============================================================================
/**
*/
class SpacedAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SpacedAudioProcessorEditor (SpacedAudioProcessor&);
    ~SpacedAudioProcessorEditor();

	void setUpAttachments();

    //==============================================================================
	void resized() override;
	void paint (Graphics&) override;

	void drawStaticUIElements(Graphics&);
	void drawNodeConnectorLines(Graphics&, int iterator);
    void drawFinalNodeConnectorLine(Graphics&);
	void drawBorderOnSelectedNode(Graphics& g, Node selectedNode);

    void mouseDown(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;

	void selectNodeForMovement(const MouseEvent&);
	void updateNodePosition(const MouseEvent&, Node&);
    void updateRoomSizeUIElements(const MouseEvent&);
	void keepNodeInField(float&, float&, Node selecedNode);

	void updateRoomSizeParameter();
	void updateMixParameter();
	void updatePanParameter();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SpacedAudioProcessor& processor;

	Font largeFont = Font(Typeface::createSystemTypefaceFor(BinaryData::RobotoThin_ttf, BinaryData::RobotoThin_ttfSize));
	Font mediumFont = Font(Typeface::createSystemTypefaceFor(BinaryData::RobotoThin_ttf, BinaryData::RobotoThin_ttfSize));
	Font smallFont = Font(Typeface::createSystemTypefaceFor(BinaryData::RobotoThin_ttf, BinaryData::RobotoThin_ttfSize));

	Colour textColour = Colour::fromRGB(252, 252, 252);
	Colour backgroundColour = Colour::fromRGB(33, 33, 33);
    Colour nodePink = Colour::fromRGB(239, 29, 129);
	Colour filterNodeColour[4] = { Colour::fromRGB(60, 176, 247),
								   Colour::fromRGB(239, 29, 129),
								   Colour::fromRGB(219, 66, 66),
								   Colour::fromRGB(221, 162, 31) };

	float initialNodeWidth = 20;
	
    float nodeFieldXOnMouseDown = 0;
    float nodeFieldYOnMouseDown = 0;
    float nodeFieldWidthOnMouseDown = 0;
    float nodeFieldHeightOnMouseDown = 0;
    float nodeFieldRightOnMouseDown = 0;
    float nodeFieldBottomOnMouseDown = 0;
	Node *selectedNode = nullptr;
	int selectedNodeIndex = 0;

	float roomSize = 0;
	float mix = 0;
	float pan = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpacedAudioProcessorEditor)
};

//===========================================================================
// Unused Elements

/*
 
 void mouseDoubleClick(const MouseEvent&) override;

 void updateNodeWidth(const MouseEvent&, Node&);

 bool keyPressed(const KeyPress&, Component *originatingComponent) override;

 */
