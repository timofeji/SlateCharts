// Copyright Drop Games Inc.


#include "RadarChart.h"

#include "UICharts/Slate/SRadarChart.h"
#define LOCTEXT_NAMESPACE "UICharts"

void URadarChart::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	SlateChart->SetBrush(&Brush);
}

void URadarChart::ReleaseSlateResources(bool bReleaseChildren)
{
	SlateChart.Reset();
}

TSharedRef<SWidget> URadarChart::RebuildWidget()
{
	SlateChart = SNew(SRadarChart)
		.Brush(&Brush);
	return SlateChart.ToSharedRef();
}

#if WITH_EDITOR
const FText URadarChart::GetPaletteCategory()
{
	return LOCTEXT("UIChartsCategory", "UICharts");
}
#endif

#undef LOCTEXT_NAMESPACE
