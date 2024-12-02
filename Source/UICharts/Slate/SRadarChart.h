// Copyright Drop Games Inc.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"


/**
 * 

 */
class UICHARTS_API SRadarChart : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRadarChart)
		{
		}

		SLATE_ATTRIBUTE(const FSlateBrush*, Brush)
	SLATE_END_ARGS()


	void SetBrush(FSlateBrush* InBrush);

	void SetData(TArray<float> InData);


	void Construct(const FArguments& InArgs);
	float EvaluateExpression(const FString& Expression);

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override;

protected:	
	TArray<float> Data;
	TArray<FText> Labels;
	float DataBounds;

private:
	FInvalidatableBrushAttribute Brush;

};
