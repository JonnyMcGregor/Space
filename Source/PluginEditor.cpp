/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SpacedAudioProcessorEditor::SpacedAudioProcessorEditor (SpacedAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
	setSize(800, 500);

	largeFont.setExtraKerningFactor(0.5);
	largeFont.setHeight(35);
	mediumFont.setExtraKerningFactor(0.25);
	mediumFont.setHeight(25);
	smallFont.setHeight(15);

	processor.reverbNode.setXPosition(proportionOfWidth(0.333f));
	processor.reverbNode.setYPosition(proportionOfHeight(0.4f));

	nodeField = Rectangle<float>(0, 0, proportionOfWidth(0.666f), proportionOfHeight(0.666f));
	addKeyListener(this);
	setWantsKeyboardFocus(true);
}

SpacedAudioProcessorEditor::~SpacedAudioProcessorEditor()
{
}

void SpacedAudioProcessorEditor::setUpAttachments()
{

}
//==============================================================================
void SpacedAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
}

//==============================================================================

void SpacedAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(backgroundColour);
	
	drawStaticUIElements(g);

	processor.reverbNode.drawNode(g);

	for (int i = 0; i < processor.filterNodes.size(); i++)
	{
		processor.filterNodes[i].drawNode(g);
		drawNodeConnectorLines(g, i);
	}

	if (selectedNode != nullptr)
	{
		drawBorderOnSelectedNode(g, *selectedNode);
	}

}

void SpacedAudioProcessorEditor::drawStaticUIElements(Graphics& g)
{
	Rectangle<float> titleFontArea = Rectangle<float>(0.0f, proportionOfHeight(0.666f), (float)getWidth(), proportionOfHeight(0.333f));
	g.setColour(textColour);
	g.setFont(largeFont);
	g.drawText("space.", titleFontArea, Justification::centred);
}

void SpacedAudioProcessorEditor::drawNodeConnectorLines(Graphics& g, int iterator)
{
	Line<float> nodeConnector = Line<float>(processor.reverbNode.getXPosition(), processor.reverbNode.getYPosition(), processor.filterNodes[iterator].getXPosition(), processor.filterNodes[iterator].getYPosition());
	ColourGradient lineGradient = ColourGradient(processor.reverbNode.getColour(), processor.reverbNode.getXPosition(), processor.reverbNode.getYPosition(),
		processor.filterNodes[iterator].getColour(), processor.filterNodes[iterator].getXPosition(), processor.filterNodes[iterator].getYPosition(), false);
	g.setGradientFill(lineGradient);
	g.drawLine(nodeConnector, 2);
}

void SpacedAudioProcessorEditor::drawBorderOnSelectedNode(Graphics& g, Node selectedNode)
{
	g.setColour(Colours::white);
	g.drawLine(selectedNode.gradientArea.getX(), selectedNode.gradientArea.getY(), selectedNode.gradientArea.getX() + selectedNode.gradientArea.getWidth() * 0.125f, selectedNode.gradientArea.getY(), 0.5);
	g.drawLine(selectedNode.gradientArea.getRight(), selectedNode.gradientArea.getY(), selectedNode.gradientArea.getRight() - selectedNode.gradientArea.getWidth() * 0.125f, selectedNode.gradientArea.getY(), 0.5);
	g.drawLine(selectedNode.gradientArea.getX(), selectedNode.gradientArea.getY(), selectedNode.gradientArea.getX(), selectedNode.gradientArea.getY() + selectedNode.gradientArea.getHeight() * 0.125f, 0.5);
	g.drawLine(selectedNode.gradientArea.getRight(), selectedNode.gradientArea.getY(), selectedNode.gradientArea.getRight(), selectedNode.gradientArea.getY() + selectedNode.gradientArea.getHeight() * 0.125f, 0.5);

	g.drawLine(selectedNode.gradientArea.getX(), selectedNode.gradientArea.getBottom(), selectedNode.gradientArea.getX() + selectedNode.gradientArea.getWidth() * 0.125f, selectedNode.gradientArea.getBottom(), 0.5);
	g.drawLine(selectedNode.gradientArea.getRight(), selectedNode.gradientArea.getBottom(), selectedNode.gradientArea.getRight() - selectedNode.gradientArea.getWidth() * 0.125f, selectedNode.gradientArea.getBottom(), 0.5);
	g.drawLine(selectedNode.gradientArea.getX(), selectedNode.gradientArea.getBottom(), selectedNode.gradientArea.getX(), selectedNode.gradientArea.getBottom() - selectedNode.gradientArea.getHeight() * 0.125f, 0.5);
	g.drawLine(selectedNode.gradientArea.getRight(), selectedNode.gradientArea.getBottom(), selectedNode.gradientArea.getRight(), selectedNode.gradientArea.getBottom() - selectedNode.gradientArea.getHeight() * 0.125f, 0.5);

}

//==============================================================================


void SpacedAudioProcessorEditor::mouseDoubleClick(const MouseEvent& m)
{
	if (m.getMouseDownX() < nodeField.getRight() && m.getMouseDownX() > nodeField.getX() &&
		m.getMouseDownY() < nodeField.getBottom() && m.getMouseDownY() > nodeField.getY())
	{
		if (numberOfVisibleFilterNodes < 4)
		{
			processor.filterNodes.push_back(Node(m.getMouseDownX(), m.getMouseDownY(), initialNodeWidth, filterNodeColour[processor.filterNodes.size()]));
			numberOfVisibleFilterNodes++;
			repaint();
			
		}
	}
}

bool SpacedAudioProcessorEditor::keyPressed(const KeyPress &key, Component* originatingComponent)
{
	if (key == KeyPress::deleteKey)
	{
		if (selectedNode != nullptr && selectedNode != &processor.reverbNode)
		{
			processor.filterNodes.pop_back();
			selectedNode = nullptr;
			numberOfVisibleFilterNodes--;
			repaint();
		}
	}
	return true;
}

void SpacedAudioProcessorEditor::mouseDrag(const MouseEvent& m)
{
	selectNodeForMovement(m);
	if (selectedNode != nullptr)
	{
		if (selectedNode == &processor.reverbNode)
		{
			updateNodeWidth(m, *selectedNode);
		}
		else
			updateNodePosition(m, *selectedNode);
	}
		
	repaint();
	updateRoomSizeParameter();
}

void SpacedAudioProcessorEditor::selectNodeForMovement(const MouseEvent &m)
{
	if (m.getMouseDownX() < processor.reverbNode.nodeArea.getRight() && m.getMouseDownX() > processor.reverbNode.nodeArea.getX() &&
		m.getMouseDownY() < processor.reverbNode.nodeArea.getBottom() && m.getMouseDownY() > processor.reverbNode.nodeArea.getY())
	{
		selectedNode = &processor.reverbNode;
	}
	else 
	{
		for (int i = 0; i < numberOfVisibleFilterNodes; i++)
		{
			//if mouse-click occurs within the node area, that node is used for movement.
			if (m.getMouseDownX() < processor.filterNodes[i].nodeArea.getRight() && m.getMouseDownX() > processor.filterNodes[i].nodeArea.getX() &&
				m.getMouseDownY() < processor.filterNodes[i].nodeArea.getBottom() && m.getMouseDownY() > processor.filterNodes[i].nodeArea.getY())
			{
				selectedNode = &processor.filterNodes[i];
				selectedNodeIndex = i;
			}
		}
	}
}

void SpacedAudioProcessorEditor::updateNodeWidth(const MouseEvent &m, Node &selectedNode)
{
	float newDiameter = m.getDistanceFromDragStart();
	if (newDiameter > 100)
	{
		newDiameter = 100;
	}

	selectedNode.setDiameter(newDiameter);
}

void SpacedAudioProcessorEditor::updateNodePosition(const MouseEvent &m, Node &selectedNode)
{
	float newX = m.getDistanceFromDragStartX() + m.getMouseDownX();
	float newY = m.getDistanceFromDragStartY() + m.getMouseDownY();

	keepNodeInField(newX, newY, selectedNode);

	selectedNode.setXPosition(newX);
	selectedNode.setYPosition(newY);

	updateRoomSizeParameter();
	updateWidthParameter();
	updateMixParameter();
	updateDampingParameter();
}



void SpacedAudioProcessorEditor::keepNodeInField(float &newX, float &newY, Node selectedNode)
{
	if (newX > nodeField.getRight())
		newX = nodeField.getRight();
	if (newX < nodeField.getX())
		newX = nodeField.getX();
	if (newY > nodeField.getBottom())
		newY = nodeField.getBottom();
	if (newY < nodeField.getY())
		newY = nodeField.getY();
}

//==============================================================================

void SpacedAudioProcessorEditor::updateRoomSizeParameter()
{
	roomSize = jmap(processor.reverbNode.getRadius(), 0.0f, 100.0f, 0.0f, 1.0f);
	processor.parameters.getParameter("roomSize")->beginChangeGesture();
	processor.parameters.getParameter("roomSize")->setValueNotifyingHost(roomSize);
	processor.parameters.getParameter("roomSize")->endChangeGesture();
}
void SpacedAudioProcessorEditor::updateWidthParameter()
{
	//width = jmap(panNodes[currentDelayNode].getYPosition(), panNodeField.getBottom(), panNodeField.getY(), 0.0f, 1.0f);
	processor.parameters.getParameter("width")->beginChangeGesture();
	processor.parameters.getParameter("width")->setValueNotifyingHost(width);
	processor.parameters.getParameter("width")->endChangeGesture();
}

void SpacedAudioProcessorEditor::updateMixParameter()
{
	//feedback = jmap(panNodes[currentDelayNode].getYPosition(), panNodeField.getBottom(), panNodeField.getY(), 0.0f, 1.0f);
	processor.parameters.getParameter("mix")->beginChangeGesture();
	processor.parameters.getParameter("mix")->setValueNotifyingHost(mix);
	processor.parameters.getParameter("mix")->endChangeGesture();
}

void SpacedAudioProcessorEditor::updateDampingParameter()
{
	//feedback = jmap(panNodes[currentDelayNode].getYPosition(), panNodeField.getBottom(), panNodeField.getY(), 0.0f, 1.0f);
	processor.parameters.getParameter("damping")->beginChangeGesture();
	processor.parameters.getParameter("damping")->setValueNotifyingHost(damping);
	processor.parameters.getParameter("damping")->endChangeGesture();
}