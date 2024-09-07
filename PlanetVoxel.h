// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "FastNoiseLite.h"
#include "GameFramework/Actor.h"
#include "PlanetVoxel.generated.h"

USTRUCT()
struct FChunkMeshDataVoxel
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
class PLANETS_API APlanetVoxel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlanetVoxel();

	UPROPERTY(EditAnywhere, Category = "Planet")
	TObjectPtr<UMaterialInterface> Material;

	UPROPERTY(EditAnywhere, Category = "Planet")
	TObjectPtr<UMaterialInterface> Material2;

	UPROPERTY(EditAnywhere, Category = "Planet")
	TObjectPtr<UMaterialInterface> Material3;

	UPROPERTY(EditAnywhere, Category = "Planet")
	float VoxelSize;


	UPROPERTY(EditAnywhere, Category = "Planet")
	int PlanetRadius;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	FChunkMeshDataVoxel MeshData;
	FChunkMeshDataVoxel MeshData2;
	FChunkMeshDataVoxel MeshData3;

	TObjectPtr<UProceduralMeshComponent> Mesh;

	TArray<int> Voxels; //compress 3d array

	FVector indexToFvector(int index);

	void addVertices(FChunkMeshDataVoxel& data, int Orientation, FVector location);
};
