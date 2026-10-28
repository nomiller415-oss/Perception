#include "LookAndFeel.h"

PerceptionLookAndFeel::PerceptionLookAndFeel(juce::Colour accentColor)
    : accent (accentColor)
{
}

PerceptionLookAndFeel::~PerceptionLookAndFeel() = default;

void PerceptionLookAndFeel::drawRotarySlider (juce::Graphics& g,
    int x, int y, int width, int height,
    float sliderPos, float rotaryStartAngle,
    float rotaryEndAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float> (static_cast<float> (x),
        static_cast<float> (y),
        static_cast<float> (width),
        static_cast<float> (height)).reduced(10.0f);

    auto center = bounds.getCentre();
    float radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;

    float ringThickness = 6.0f;

    juce::Path backgroundArc;

    backgroundArc.addCentredArc (center.x, center.y,
        radius, radius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);

    g.setColour (juce::Colour (0xff2A2F35));

    g.strokePath (
        backgroundArc,
        juce::PathStrokeType (
            ringThickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));


    auto angle =
        juce::jmap (
            sliderPos,
            rotaryStartAngle,
            rotaryEndAngle);

    juce::Path valueArc;

    valueArc.addCentredArc (
        center.x,
        center.y,
        radius,
        radius,
        0.0f,
        rotaryStartAngle,
        angle,
        true);

    g.setColour (accent);

    g.strokePath (
        valueArc,
        juce::PathStrokeType (
            ringThickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));


    auto knobBounds =
        bounds.reduced (16.0f);

    g.setColour (
        juce::Colour (0xff1A1D22));

    g.fillEllipse (knobBounds);

    g.setColour (
        juce::Colour (0xff333840));

    g.drawEllipse (
        knobBounds,
        1.5f);

    
    const float indicatorLength =
        knobBounds.getWidth() * 0.35f;

    juce::Path indicator;

    indicator.startNewSubPath (
        0.0f,
        -indicatorLength);

    indicator.lineTo (
        0.0f,
        -indicatorLength * 0.55f);

    juce::AffineTransform transform;

    transform =
        transform.rotated (angle);

    transform =
        transform.translated (
            center.x,
            center.y);

    indicator.applyTransform (transform);

    g.setColour (accent);

    g.strokePath (
        indicator,
        juce::PathStrokeType (
            3.0f,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));



    g.setColour (
        accent.withAlpha (0.8f));

    g.fillEllipse (
        center.x - 2.5f,
        center.y - 2.5f,
        5.0f,
        5.0f);

}

juce::Colour PerceptionLookAndFeel::getAccentColor() const
{
    return accent;
}