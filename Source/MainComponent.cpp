/*
 ==============================================================================
 This file was auto-generated!
 ==============================================================================
 */

#include "MainComponent.h"

//==============================================================================

void MainComponent::sliderValueChanged (Slider* slider)
{
    if (slider == &freqSlider){
        frequency = freqSlider.getValue();
    } else if (slider == &ampSlider){
        amplitude = ampSlider.getValue();
    }
}

void MainComponent::updateFrequency(){
    
    increment = frequency * wtSize / currentSampleRate;
    phase = fmod((phase + increment), wtSize);
    
}


MainComponent::MainComponent()
{
    
    setSize (800, 600);
    
    freqSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    freqSlider.setRange(50.0, 500.0);
    freqSlider.setTextValueSuffix("Hz");
    freqSlider.addListener(this);
    freqSlider.setValue(200);
    addAndMakeVisible(freqSlider);
    freqLabel.setText("Frequency:", dontSendNotification);
    freqLabel.attachToComponent(&freqSlider, true);
    
    ampSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    ampSlider.setRange(0.0, 1.0);
    ampSlider.addListener(this);
    ampSlider.setValue(0.0);
    addAndMakeVisible(ampSlider);
    ampLabel.setText("Amplitude:", dontSendNotification);
    ampLabel.attachToComponent(&ampSlider, true);
    
    // specify the number of input and output channels that we want to open
    setAudioChannels (2, 2);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    frequency = freqSlider.getValue();
    phase = 0;
    wtSize = 1024;
    amplitude = ampSlider.getValue();
    currentSampleRate = sampleRate;
    
    for (int i = 0; i < wtSize; i++)
    {
        waveTable.insert(i, sin(2.0 * double_Pi * i /wtSize));
    }
    
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    
    float* const leftSpeaker = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    float* const rightSpeaker = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    
    for (int sample=0; sample < bufferToFill.numSamples; sample++)
    {
        leftSpeaker[sample] = waveTable[(int)phase] * amplitude;
        rightSpeaker[sample] = waveTable[(int)phase] * amplitude;
        updateFrequency();
        
    }
    
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    
    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::seagreen);
    
    g.setFont (Font (69.0f));
    g.setColour (Colours::red);
    g.drawText ("H A R T", getLocalBounds(), Justification::centred, true);
    
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    const int labelSpace = 100;
    freqSlider.setBounds(labelSpace, 20, getWidth() - 100, 20);
    ampSlider.setBounds(labelSpace, 50, getWidth() - 100, 50);
    
    
}

