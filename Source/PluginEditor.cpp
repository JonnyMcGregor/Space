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
    smallFont.setExtraKerningFactor(0.125);
	
	
    if(processor.isFirstTimeOpeningEditor)
    {
        processor.nodeField = Rectangle<float>(proportionOfWidth(0.5f) - (proportionOfWidth(0.35f)/2), proportionOfHeight(0.4f) - (proportionOfHeight(0.417f)/2), proportionOfWidth(0.35f), proportionOfHeight(0.417f));
       
        processor.reverbNode.setXPosition(processor.nodeField.getCentreX());
        processor.reverbNode.setYPosition(processor.nodeField.getCentreY());

        processor.roomSizeNodes.push_back(Node(processor.nodeField.getX(), processor.nodeField.getY(), initialNodeWidth,
            nodePink, backgroundColour));
        processor.roomSizeNodes.push_back(Node(processor.nodeField.getX(), processor.nodeField.getBottom(), initialNodeWidth,
            nodePink, backgroundColour));
        processor.roomSizeNodes.push_back(Node(processor.nodeField.getRight(), processor.nodeField.getBottom(), initialNodeWidth, nodePink, backgroundColour));
        processor.roomSizeNodes.push_back(Node(processor.nodeField.getRight(), processor.nodeField.getY(), initialNodeWidth, nodePink, backgroundColour));
        
        processor.isFirstTimeOpeningEditor = false;
        
        updateMixParameter();
        updatePanParameter();
        updateWidthParameter();
        updateRoomSizeParameter();
    }
}

SpacedAudioProcessorEditor::~SpacedAudioProcessorEditor()
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
	
    processor.reverbNode.drawNode(g);
    
    drawDryWetLine(g);
    
	for (int i = 0; i < processor.roomSizeNodes.size(); i++)
	{
		processor.roomSizeNodes[i].drawNode(g);
        if(i != 0)
            drawNodeConnectorLines(g, i);
	}
    
    drawFinalNodeConnectorLine(g);
    
    drawStaticUIElements(g);

	if (selectedNode != nullptr)
	{
		drawBorderOnSelectedNode(g, *selectedNode);
	}

}

void SpacedAudioProcessorEditor::drawDryWetLine(Graphics& g)
{
    waveformPathLeft.clear();
    waveformPathRight.clear();
    
    setDryWetLineStartPositions();
    for (int i = 1; i <= processor.nodeField.getWidth(); i++)
    {
        setWaveformPath(g, i);
    }
    g.strokePath(waveformPathLeft, PathStrokeType(2.0f));
    g.strokePath(waveformPathRight, PathStrokeType(2.0f));

}
void SpacedAudioProcessorEditor::setDryWetLineStartPositions()
{
    waveformPathLeft.startNewSubPath(processor.nodeField.getX(), processor.reverbNode.getYPosition());
    waveformPathRight.startNewSubPath(processor.nodeField.getRight(), processor.reverbNode.getYPosition());
}

void SpacedAudioProcessorEditor::setWaveformPath(Graphics & g, int i)
{
    if(processor.nodeField.getX() + i > (processor.reverbNode.getXPosition() + processor.reverbNode.getRadius()))
    {
        
        float yValue = 8 * sin(MathConstants<float>::twoPi * (processor.nodeField.getRight() - (processor.nodeField.getX() + i - processor.reverbNode.getXPosition() - processor.reverbNode.getRadius())) * 0.015) + processor.reverbNode.getYPosition();
        
        if(yValue > processor.nodeField.getBottom())
        {
            yValue = processor.nodeField.getBottom();
        }
        else if(yValue < processor.nodeField.getY())
        {
            yValue = processor.nodeField.getY();
        }
        
        waveformPathRight.lineTo(processor.nodeField.getRight() - (processor.nodeField.getX() + i - processor.reverbNode.getXPosition() - processor.reverbNode.getRadius()), yValue);
        
    }
    else if(processor.nodeField.getX() + i < processor.reverbNode.getXPosition() - processor.reverbNode.getRadius())
    {
        float yValue = 8 * sin(MathConstants<float>::twoPi * i * 0.015) + processor.reverbNode.getYPosition();
        
        if(yValue > processor.nodeField.getBottom())
        {
            yValue = processor.nodeField.getBottom();
        }
        else if(yValue < processor.nodeField.getY())
        {
            yValue = processor.nodeField.getY();
        }
        
        waveformPathLeft.lineTo(processor.nodeField.getX() + i, yValue);
        
    }
}

void SpacedAudioProcessorEditor::drawNodeConnectorLines(Graphics& g, int iterator)
{
    Line<float> nodeConnector = Line<float>(processor.roomSizeNodes[iterator].getXPosition(), processor.roomSizeNodes[iterator].getYPosition(), processor.roomSizeNodes[iterator-1].getXPosition(), processor.roomSizeNodes[iterator-1].getYPosition());
    
    g.setColour(nodePink);
    g.drawLine(nodeConnector, 2);
}

void SpacedAudioProcessorEditor::drawFinalNodeConnectorLine(Graphics &g)
{
     Line<float> nodeConnector = Line<float>(processor.roomSizeNodes.back().getXPosition(), processor.roomSizeNodes.back().getYPosition(), processor.roomSizeNodes[0].getXPosition(), processor.roomSizeNodes[0].getYPosition());
     g.drawLine(nodeConnector, 2);
}

void SpacedAudioProcessorEditor::drawStaticUIElements(Graphics& g)
{
    //Initialise Elements
    Rectangle<float> titleFontArea = Rectangle<float>(15.0f, proportionOfHeight(0.75f), (float)getWidth(), proportionOfHeight(0.25f));
    
    Line<float> arrowLine1 = Line<float>(processor.nodeField.getRight() + 25, processor.nodeField.getBottom() - 10, processor.nodeField.getRight() + 25, processor.nodeField.getY() + 10);
    Line<float> arrowLine2 = Line<float>(processor.nodeField.getRight() + 25, processor.nodeField.getY() + 10, processor.nodeField.getRight() + 25, processor.nodeField.getBottom() - 10);
    Line<float> arrowLine3 = Line<float>(processor.nodeField.getX() + 12, processor.nodeField.getBottom() + 25, processor.nodeField.getRight() - 10, processor.nodeField.getBottom() + 25);
    Line<float> arrowLine4 = Line<float>(processor.nodeField.getRight() - 12, processor.nodeField.getBottom() + 25, processor.nodeField.getX() + 10, processor.nodeField.getBottom() + 25);
    
    //Draw Elements
    g.setColour(textColour);
    g.setFont(largeFont);
    g.drawText("space.", titleFontArea, Justification::centred);
    
    g.setFont(smallFont);
    
    g.drawText("wet", processor.nodeField.getX(), processor.nodeField.getY(), processor.nodeField.getWidth(), 40, Justification::centred);
    g.drawText("dry", processor.nodeField.getX(), processor.nodeField.getBottom() - 40, processor.nodeField.getWidth(), 40, Justification::centred);
    
    g.drawArrow(arrowLine1, 1.0f, 4.0f, 10.0f);
    g.drawArrow(arrowLine2, 1.0f, 4.0f, 10.0f);
    g.drawArrow(arrowLine3, 1.0f, 4.0f, 10.0f);
    g.drawArrow(arrowLine4, 1.0f, 4.0f, 10.0f);

    g.addTransform(AffineTransform::rotation(-MathConstants<float>::halfPi, processor.nodeField.getRight(), processor.nodeField.getBottom()));
    
    g.drawText("room length", processor.nodeField.getRight(), processor.nodeField.getBottom(), processor.nodeField.getHeight(), 100, Justification::centred);
    
    g.addTransform(AffineTransform::rotation(MathConstants<float>::halfPi, processor.nodeField.getRight(), processor.nodeField.getBottom()));
    
    g.drawText("room width", processor.nodeField.getX(), processor.nodeField.getBottom(), processor.nodeField.getWidth(), 100, Justification::centred);
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
void SpacedAudioProcessorEditor::mouseDown(const MouseEvent & m)
{
    selectNodeForMovement(m);
    
    previousMixValue = mix;
    previousPanValue = pan;
    
    nodeFieldWidthOnMouseDown = processor.nodeField.getWidth();
    nodeFieldHeightOnMouseDown = processor.nodeField.getHeight();
    nodeFieldXOnMouseDown = processor.nodeField.getX();
    nodeFieldYOnMouseDown = processor.nodeField.getY();
    nodeFieldRightOnMouseDown = processor.nodeField.getRight();
    nodeFieldBottomOnMouseDown = processor.nodeField.getBottom();
    
    repaint();
}

void SpacedAudioProcessorEditor::mouseDrag(const MouseEvent& m)
{
    if(m.mouseWasDraggedSinceMouseDown())
    {
        selectNodeForMovement(m);
        waveformPathLeft.clear();
        waveformPathRight.clear();

        if (selectedNode != nullptr)
        {
            if (selectedNode != &processor.reverbNode)
            {
                updateRoomSizeUIElements(m);
            }
            else
                updateNodePosition(m, *selectedNode);
        }
        
        repaint();
        updateRoomSizeParameter();
        updateMixParameter();
        updateWidthParameter();
        updatePanParameter();
    }
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
		for (int i = 0; i < processor.roomSizeNodes.size(); i++)
		{
			//if mouse-click occurs within the node area, that node is used for movement.
			if (m.getMouseDownX() < processor.roomSizeNodes[i].nodeArea.getRight() && m.getMouseDownX() > processor.roomSizeNodes[i].nodeArea.getX() &&
				m.getMouseDownY() < processor.roomSizeNodes[i].nodeArea.getBottom() && m.getMouseDownY() > processor.roomSizeNodes[i].nodeArea.getY())
			{
				selectedNode = &processor.roomSizeNodes[i];
				selectedNodeIndex = i;
			}
		}
	}
    
}

void SpacedAudioProcessorEditor::updateNodePosition(const MouseEvent &m, Node &selectedNode)
{
	float newX = m.getDistanceFromDragStartX() + m.getMouseDownX();
	float newY = m.getDistanceFromDragStartY() + m.getMouseDownY();

	keepNodeInField(newX, newY, selectedNode);

	selectedNode.setXPosition(newX);
	selectedNode.setYPosition(newY);
}

void SpacedAudioProcessorEditor::updateRoomSizeUIElements(const MouseEvent &m)
{
    float newX {0}, newY {0}, newRight {0}, newBottom {0}, newWidth {0}, newHeight{0};
    
    setNewRoomSizeValues(newX, newY, newRight, newBottom, m);
    
    newWidth = newRight - newX;
    newHeight = newBottom - newY;
    
    if (newWidth < proportionOfWidth(0.5) && newWidth > proportionOfWidth(0.25))
    {
        processor.nodeField.setX(newX);
        processor.nodeField.setRight(newRight);
        
        processor.roomSizeNodes[0].setXPosition(processor.nodeField.getX());
        processor.roomSizeNodes[1].setXPosition(processor.nodeField.getX());
        processor.roomSizeNodes[2].setXPosition(processor.nodeField.getRight());
        processor.roomSizeNodes[3].setXPosition(processor.nodeField.getRight());
    }
    if(newHeight < proportionOfHeight(0.666) && newHeight > proportionOfHeight(0.25))
    {
        processor.nodeField.setY(newY);
        processor.nodeField.setBottom(newBottom);
        
        processor.roomSizeNodes[0].setYPosition(processor.nodeField.getY());
        processor.roomSizeNodes[1].setYPosition(processor.nodeField.getBottom());
        processor.roomSizeNodes[2].setYPosition(processor.nodeField.getBottom());
        processor.roomSizeNodes[3].setYPosition(processor.nodeField.getY());
    }
    
    processor.reverbNode.setXPosition(jmap(previousPanValue, 0.0f, 1.0f, processor.nodeField.getX(), processor.nodeField.getRight()));
    processor.reverbNode.setYPosition(jmap(previousMixValue, 0.0f, 1.0f, processor.nodeField.getBottom(), processor.nodeField.getY()));
}

void SpacedAudioProcessorEditor::setNewRoomSizeValues(float& x, float& y, float& right, float& bottom, const MouseEvent& m)
{
    if(selectedNode == &processor.roomSizeNodes[0])
    {
        x = nodeFieldXOnMouseDown + m.getDistanceFromDragStartX()/2;
        y = nodeFieldYOnMouseDown + m.getDistanceFromDragStartY()/2;
        right = nodeFieldRightOnMouseDown - m.getDistanceFromDragStartX()/2;
        bottom = nodeFieldBottomOnMouseDown - m.getDistanceFromDragStartY()/2;
    }
    else if(selectedNode == &processor.roomSizeNodes[1])
    {
        x = nodeFieldXOnMouseDown + m.getDistanceFromDragStartX()/2;
        y = nodeFieldYOnMouseDown - m.getDistanceFromDragStartY()/2;
        right = nodeFieldRightOnMouseDown - m.getDistanceFromDragStartX()/2;
        bottom = nodeFieldBottomOnMouseDown + m.getDistanceFromDragStartY()/2;
    }
    else if(selectedNode == &processor.roomSizeNodes[2])
    {
        x = nodeFieldXOnMouseDown - m.getDistanceFromDragStartX()/2;
        y = nodeFieldYOnMouseDown - m.getDistanceFromDragStartY()/2;
        right = nodeFieldRightOnMouseDown + m.getDistanceFromDragStartX()/2;
        bottom = nodeFieldBottomOnMouseDown + m.getDistanceFromDragStartY()/2;
    }
    else if(selectedNode == &processor.roomSizeNodes[3])
    {
        x = nodeFieldXOnMouseDown - m.getDistanceFromDragStartX()/2;
        y = nodeFieldYOnMouseDown + m.getDistanceFromDragStartY()/2;
        right = nodeFieldRightOnMouseDown + m.getDistanceFromDragStartX()/2;
        bottom = nodeFieldBottomOnMouseDown - m.getDistanceFromDragStartY()/2;
    }
    
}
void SpacedAudioProcessorEditor::keepNodeInField(float &newX, float &newY, Node selectedNode)
{
	if (newX > processor.nodeField.getRight())
		newX = processor.nodeField.getRight();
	if (newX < processor.nodeField.getX())
		newX = processor.nodeField.getX();
	if (newY > processor.nodeField.getBottom())
		newY = processor.nodeField.getBottom();
	if (newY < processor.nodeField.getY())
		newY = processor.nodeField.getY();
}

//==============================================================================

void SpacedAudioProcessorEditor::updateRoomSizeParameter()
{
	roomSize = jmap(processor.nodeField.getHeight(), (float)proportionOfHeight(0.25), (float)proportionOfHeight(0.666), 0.0f, 1.0f);
	processor.parameters.getParameter("roomSize")->beginChangeGesture();
	processor.parameters.getParameter("roomSize")->setValueNotifyingHost(roomSize);
	processor.parameters.getParameter("roomSize")->endChangeGesture();
}

void SpacedAudioProcessorEditor::updateMixParameter()
{
	mix = jmap(processor.reverbNode.getYPosition(), processor.nodeField.getBottom(), processor.nodeField.getY(), 0.0f, 1.0f);
	processor.parameters.getParameter("mix")->beginChangeGesture();
	processor.parameters.getParameter("mix")->setValueNotifyingHost(mix);
	processor.parameters.getParameter("mix")->endChangeGesture();
}

void SpacedAudioProcessorEditor::updateWidthParameter()
{
	width = jmap(processor.nodeField.getWidth(), (float)proportionOfWidth(0.25),(float)proportionOfWidth(0.5), 0.0f, 1.0f);
	processor.parameters.getParameter("width")->beginChangeGesture();
	processor.parameters.getParameter("width")->setValueNotifyingHost(width);
	processor.parameters.getParameter("width")->endChangeGesture();
}

void SpacedAudioProcessorEditor::updatePanParameter()
{
    pan = jmap(processor.reverbNode.getXPosition(), processor.nodeField.getX(),processor.nodeField.getRight(), 0.0f, 1.0f);
    processor.parameters.getParameter("pan")->beginChangeGesture();
    processor.parameters.getParameter("pan")->setValueNotifyingHost(pan);
    processor.parameters.getParameter("pan")->endChangeGesture();
}

//==============================================================================

//Unused Elements

/*
 
 void SpacedAudioProcessorEditor::mouseDoubleClick(const MouseEvent& m)
 {
 if (m.getMouseDownX() < nodeField.getRight() && m.getMouseDownX() > nodeField.getX() &&
 m.getMouseDownY() < nodeField.getBottom() && m.getMouseDownY() > nodeField.getY())
 {
 if (numberOfVisibleFilterNodes < 4)
 {
 processor.roomSizeNodes.push_back(Node(m.getMouseDownX(), m.getMouseDownY(), initialNodeWidth, filterNodeColour[processor.roomSizeNodes.size()], backgroundColour));
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
 processor.roomSizeNodes.pop_back();
 selectedNode = nullptr;
 numberOfVisibleFilterNodes--;
 repaint();
 }
 }
 return true;
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
 
 */
