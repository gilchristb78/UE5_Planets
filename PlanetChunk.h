// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "FastNoiseLite.h"
#include "PlanetChunk.generated.h"

USTRUCT()
struct FChunkMeshDataPlanet
{
	GENERATED_BODY();

public:
	TArray<FVector> Vertices;
	TArray<FVector> WaterVertices;
	TArray<int> Triangles;
	TArray<FVector> Normals;
	TArray<FVector> WaterNormals;
	TArray<FVector2D> UV0;
	TArray<FVector2D> UVX;
	TArray<FVector2D> UVY;
	TArray<FVector2D> UVZ;
	TArray<FColor> Colors;

};

UCLASS()
class PLANETS_API APlanetChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlanetChunk();

	UPROPERTY(EditAnywhere, Category = "Planet")
	TObjectPtr<UMaterialInterface> Material;

	UPROPERTY(EditAnywhere, Category = "Planet")
	TObjectPtr<UMaterialInterface> WaterMaterial;


	UPROPERTY(EditAnywhere, Category = "Planet")
	int size = 1000;

	UPROPERTY(EditAnywhere, Category = "Planet")
	int subdivisions = 2;

	UPROPERTY(EditAnywhere, Category = "Noise")
	float Frequency = 0.0015f;

	UPROPERTY(EditAnywhere, Category = "Noise")
	int FractalOctaves = 3;

	UPROPERTY(EditAnywhere, Category = "Noise")
	int NoiseSeed = 0;

	UPROPERTY(EditAnywhere, Category = "Noise")
	float FractalLacunarity = 0.6f;

	UPROPERTY(EditAnywhere, Category = "Noise")
	float FractalGain = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Noise")
	float warpScale = 80;

	UPROPERTY(EditAnywhere, Category = "Noise")
	float Frequency2 = 0.0015f;

	UPROPERTY(EditAnywhere, Category = "Noise")
	int FractalOctaves2 = 3;

	UPROPERTY(EditAnywhere, Category = "Noise")
	int NoiseSeed2 = 0;

	UPROPERTY(EditAnywhere, Category = "Noise")
	float FractalLacunarity2 = 0.6f;

	UPROPERTY(EditAnywhere, Category = "Noise")
	float FractalGain2 = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Noise")
	float warpScale2 = 80;


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	FChunkMeshDataPlanet MeshData;

	TObjectPtr<UProceduralMeshComponent> Mesh;

	TArray<TArray<FVector>> BaseTriangles = {
		{FVector::UpVector, FVector::ForwardVector, FVector::RightVector},
		{FVector::UpVector, FVector::RightVector, FVector::BackwardVector},
		{FVector::UpVector, FVector::BackwardVector, FVector::LeftVector},
		{FVector::UpVector, FVector::LeftVector, FVector::ForwardVector},
		{FVector::RightVector, FVector::ForwardVector, FVector::DownVector},
		{FVector::BackwardVector, FVector::RightVector, FVector::DownVector},
		{FVector::LeftVector, FVector::BackwardVector, FVector::DownVector},
		{FVector::ForwardVector, FVector::LeftVector, FVector::DownVector},
	};

	TArray<FVector> Equator = {
		FVector::ForwardVector,
		FVector::RightVector,
		FVector::BackwardVector,
		FVector::LeftVector 
	};

	void addVertices();
	void addTriangles();
	void addNoise();

	FastNoiseLite* Noise;
	FastNoiseLite* Noise2;

	void setNoiseVariables(float Freq, int Octaves, int Seed, float Lac, float Gain, float warp);
	void setNoiseVariables2(float Freq, int Octaves, int Seed, float Lac, float Gain, float warp);

};
