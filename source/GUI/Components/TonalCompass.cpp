#include "TonalCompass.h"
#include "../Styles/ColorPalette.h"

TonalCompass::TonalCompass()
{
}

void TonalCompass::setValues (float newBody,
    float newWarmth,
    float newPresence,
    float newAir)
{
    body = juce::jlimit (0.0f, 1.0f, newBody);
    warmth = juce::jlimit (0.0f, 1.0f, newWarmth);
    presence = juce::jlimit (0.0f, 1.0f, newPresence);
    air = juce::jlimit (0.0f, 1.0f, newAir);

    repaint();
}

void TonalCompass::resized()
{
}

void TonalCompass::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff121417));

    const auto componentBounds = getLocalBounds().toFloat();

    auto compassBounds =
        componentBounds.reduced (80.0f);

    auto center = compassBounds.getCentre();

    const float radius =
        juce::jmin (compassBounds.getWidth(),
            compassBounds.getHeight())
        * 0.45f;

    for (int i = 1; i <= 3; ++i)
    {
        const float ringRadius =
            radius * (static_cast<float> (i) / 3.0f);

        g.setColour (
            juce::Colours::white.withAlpha (0.15f));

        g.drawEllipse (
            center.x - ringRadius,
            center.y - ringRadius,
            ringRadius * 2.0f,
            ringRadius * 2.0f,
            1.0f);
    }

    const auto topAxis =
        center + juce::Point<float> (0.0f, -radius);

    const auto rightAxis =
        center + juce::Point<float> (radius, 0.0f);

    const auto bottomAxis =
        center + juce::Point<float> (0.0f, radius);

    const auto leftAxis =
        center + juce::Point<float> (-radius, 0.0f);


    g.setColour (
        juce::Colours::white.withAlpha (0.15f));

    g.drawLine (leftAxis.x,
        leftAxis.y,
        rightAxis.x,
        rightAxis.y,
        1.5f);

    g.drawLine (topAxis.x,
        topAxis.y,
        bottomAxis.x,
        bottomAxis.y,
        1.5f);


    juce::Path referenceShape;

    referenceShape.startNewSubPath (topAxis);
    referenceShape.lineTo (rightAxis);
    referenceShape.lineTo (bottomAxis);
    referenceShape.lineTo (leftAxis);
    referenceShape.closeSubPath();

    g.setColour (juce::Colour (0xff24282d));

    g.strokePath (referenceShape,
        juce::PathStrokeType (1.0f));


    constexpr float minimumExtent = 0.25f;
    constexpr float extentRange = 0.75f;

    auto getExtent = [] (float value) {
        constexpr float minimumExtent = 0.25f;
        constexpr float extentRange = 0.75f;

        return minimumExtent + value * extentRange;
    };

    const float airExtent = getExtent (air);
    const float presenceExtent = getExtent (presence);
    const float bodyExtent = getExtent (body);
    const float warmthExtent = getExtent (warmth);
    auto top =
        center + juce::Point<float> (0.0f, -radius * airExtent);

    auto right =
        center + juce::Point<float> (radius * presenceExtent, 0.0f);

    auto bottom =
        center + juce::Point<float> (0.0f, radius * bodyExtent);

    auto left =
        center + juce::Point<float> (-radius * warmthExtent, 0.0f);


    juce::Path tonalShape;

    tonalShape.startNewSubPath (top);
    tonalShape.lineTo (right);
    tonalShape.lineTo (bottom);
    tonalShape.lineTo (left);
    tonalShape.closeSubPath();



    //Air Gradient
    {
        juce::ColourGradient gradient (
            ColorPalette::airColour.withAlpha (0.08f + 0.50f * air),
            center.x,
            topAxis.y,

            juce::Colours::transparentBlack,
            center.x,
            center.y,

            false);

        g.setGradientFill (gradient);
        g.fillPath (tonalShape);
    }


    // Presence Gradient
    {
        juce::ColourGradient gradient (
            ColorPalette::presenceColour.withAlpha (0.08f + 0.50f * presence),
            rightAxis.x,
            center.y,

            juce::Colours::transparentBlack,
            center.x,
            center.y,

            false);

        g.setGradientFill (gradient);
        g.fillPath (tonalShape);
    }

    // Body Gradient
    {
        juce::ColourGradient gradient (
            ColorPalette::bodyColour.withAlpha (0.08f + 0.50f * body),
            center.x,
            bottomAxis.y,

            juce::Colours::transparentBlack,
            center.x,
            center.y,

            false);

        g.setGradientFill (gradient);
        g.fillPath (tonalShape);
    }

    // Warmth Gradient
    {
        juce::ColourGradient gradient (
            ColorPalette::warmthColour.withAlpha (0.08f + 0.50f * warmth),
            leftAxis.x,
            center.y,

            juce::Colours::transparentBlack,
            center.x,
            center.y,

            false);

        g.setGradientFill (gradient);
        g.fillPath (tonalShape);
    }

    // Glass Overlay
    g.setColour (juce::Colours::white.withAlpha (0.04f));
    g.fillPath (tonalShape);


    //Outline Glow

    {
        auto glowPath = tonalShape;

        glowPath.applyTransform (
            juce::AffineTransform::translation (
                0.0f,
                -2.0f));

        g.setColour (
            ColorPalette::airColour.withAlpha (0.01f + 0.05f * air));

        g.strokePath (
            glowPath,
            juce::PathStrokeType (10.0f));
    }

    {
        auto glowPath = tonalShape;

        glowPath.applyTransform (
            juce::AffineTransform::translation (
                -2.0f,
                0.0f));

        g.setColour (
            ColorPalette::warmthColour.withAlpha (0.01f + 0.05f * warmth));

        g.strokePath (
            glowPath,
            juce::PathStrokeType (10.0f));
    }

    {
        auto glowPath = tonalShape;

        glowPath.applyTransform (
            juce::AffineTransform::translation (
                2.0f,
                0.0f));

        g.setColour (
            ColorPalette::presenceColour.withAlpha (0.01f + 0.05f * presence));

        g.strokePath (
            glowPath,
            juce::PathStrokeType (10.0f));
    }

    {
        auto glowPath = tonalShape;

        glowPath.applyTransform (
            juce::AffineTransform::translation (
                0.0f,
                2.0f));

        g.setColour (
            ColorPalette::bodyColour.withAlpha (0.01f + 0.05f * body));

        g.strokePath (
            glowPath,
            juce::PathStrokeType (10.0f));
    }

    //Pass 2 of Glow
    {
        auto glowPath = tonalShape;

        glowPath.applyTransform (
            juce::AffineTransform::translation (
                0.0f,
                -1.0f));

        g.setColour (
            ColorPalette::airColour.withAlpha (0.03f + 0.08f * air));

        g.strokePath (
            glowPath,
            juce::PathStrokeType (6.0f));
    }

    {
        auto glowPath = tonalShape;

        glowPath.applyTransform (
            juce::AffineTransform::translation (
                1.0f,
                0.0f));

        g.setColour (
            ColorPalette::warmthColour.withAlpha (0.03f + 0.08f * warmth));

        g.strokePath (
            glowPath,
            juce::PathStrokeType (6.0f));
    }

    {
        auto glowPath = tonalShape;

        glowPath.applyTransform (
            juce::AffineTransform::translation (
                1.0f,
                0.0f));

        g.setColour (
            ColorPalette::presenceColour.withAlpha (0.03f + 0.08f * presence));

        g.strokePath (
            glowPath,
            juce::PathStrokeType (6.0f));
    }

    {
        auto glowPath = tonalShape;

        glowPath.applyTransform (
            juce::AffineTransform::translation (
                0.0f,
                1.0f));

        g.setColour (
            ColorPalette::bodyColour.withAlpha (0.03f + 0.08f * body));

        g.strokePath (
            glowPath,
            juce::PathStrokeType (6.0f));
    }



    //Outline
    auto outlineColour = ColorPalette::airColour
            .interpolatedWith (
                ColorPalette::presenceColour,
                0.33f)
            .interpolatedWith (
                ColorPalette::warmthColour,
                0.66f)
            .interpolatedWith (
                ColorPalette::bodyColour,
                0.50f);

    g.setColour (outlineColour.withAlpha (0.2f));
    g.strokePath (
        tonalShape,
        juce::PathStrokeType (2.5f));


    g.setFont (
        juce::FontOptions (18.0f));

    const int labelHeight = 24;
    const int descriptorHeight = 16;

    //Inner Disc
    g.setColour (
        juce::Colour (0xff20242A)); 

    g.fillEllipse (
        center.x - 26.0f,
        center.y - 26.0f,
        52.0f,
        52.0f);

    //Border 
    g.setColour (
        juce::Colour (0xff4A525C));

    g.drawEllipse (
        center.x - 26.0f,
        center.y - 26.0f,
        52.0f,
        52.0f,
        1.5f);

    //Tiny Ring
    g.setColour (
        juce::Colours::white.withAlpha (0.03f));

    g.fillEllipse (
        center.x - 18.0f,
        center.y - 18.0f,
        36.0f,
        36.0f);

    //Center Title
    g.setFont (juce::FontOptions (16.0f));
    g.setColour (juce::Colours::white.withAlpha (0.45f));
    g.drawText ("LIQUID PERCEPTION",
        juce::Rectangle<int> (
            static_cast<int>(center.x - 76.0f),
            static_cast<int>(center.y - 12.0f),
            160,
            24),
        juce::Justification::centred);

    // Air
    g.setFont (juce::FontOptions (18.0f));
    g.setColour (ColorPalette::airColour);
    g.drawText (
        "AIR",
        juce::Rectangle<int> (
            0,
            8,
            getWidth(),
            labelHeight),
        juce::Justification::centred);
    g.setFont (juce::FontOptions (11.0f));
    g.setColour (ColorPalette::airColour.withAlpha (0.65f));
    g.drawText (
        "Open",
        juce::Rectangle<int> (
            0,
            28,
            getWidth(),
            descriptorHeight),
        juce::Justification::centred);

    // Body
    g.setFont (juce::FontOptions (18.0f));
    g.setColour (ColorPalette::bodyColour);
    g.drawText (
        "BODY",
        juce::Rectangle<int> (
            0,
            getHeight() - 60,
            getWidth(),
            labelHeight),
        juce::Justification::centred);
    g.setFont (juce::FontOptions (11.0f));
    g.setColour (ColorPalette::bodyColour.withAlpha (0.65f));
    g.drawText (
        "Full",
        juce::Rectangle<int> (
            0,
            getHeight() - 40,
            getWidth(),
            descriptorHeight),
        juce::Justification::centred);

    // Warmth
    g.setFont (juce::FontOptions (18.0f));
    g.setColour (ColorPalette::warmthColour);
    g.drawText (
        "WARMTH",
        juce::Rectangle<int> (
            10,
            getHeight() / 2 - 12,
            120,
            labelHeight),
        juce::Justification::centredLeft);
    g.setFont (juce::FontOptions (11.0f));
    g.setColour (ColorPalette::warmthColour.withAlpha (0.65f));
    g.drawText (
        "Rich",
        juce::Rectangle<int> (
            10,
            getHeight() / 2 + 8,
            120,
            descriptorHeight),
        juce::Justification::centredLeft);

    // Presence
    g.setColour (ColorPalette::presenceColour);
    g.drawText (
        "PRESENCE",
        juce::Rectangle<int> (
            getWidth() - 130,
            getHeight() / 2 - 12,
            120,
            labelHeight),
        juce::Justification::centredRight);
    g.setFont (juce::FontOptions (11.0f));
    g.setColour (ColorPalette::presenceColour.withAlpha (0.65f));
    g.drawText (
        "Forward",
        juce::Rectangle<int> (
            getWidth() - 130,
            getHeight() / 2 + 8,
            120,
            descriptorHeight),
        juce::Justification::centredRight);
}