// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "FastNoiseLite.h"
#include "GameFramework/Actor.h"
#include "PlanetVoxel.generated.h"

UENUM(BlueprintType)
enum Shape
{
	CUBE,
	SLOPE,
	CORNER,
	CORNERSLOPE,
	HALF,
	CONE,
	null
};

UENUM(BlueprintType)
enum Direction
{
	UpBack,
	UpRight,
	UpForward,
	UpLeft,
	ForwardLeft,
	ForwardUp,
	ForwardRight,
	ForwardDown,
	LeftBack,
	LeftUp,
	LeftForward,
	LeftDown,
	BackRight,
	BackDown,
	BackLeft,
	BackUp,
	RightForward, 
	RightDown,
	RightBack,
	RightUp,
	DownBack,
	DownRight,
	DownForward,
	DownLeft 
};

USTRUCT()
struct FBlock
{
	GENERATED_BODY();

public:
	Shape shape = null;
	Direction direction = UpBack;

	FBlock(Shape NewShape = null, Direction NewDirection = UpBack)
	{
		shape = NewShape;
		direction = NewDirection;
	}
};

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

USTRUCT()
struct FMask
{
	GENERATED_BODY();

public:
	FBlock Block;
	int Normal;
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

	/*UPROPERTY(EditAnywhere, Category = "Planet")
	TObjectPtr<UMaterialInterface> Material4;

	UPROPERTY(EditAnywhere, Category = "Planet")
	TObjectPtr<UMaterialInterface> Material5;*/

	UPROPERTY(EditAnywhere, Category = "Planet")
	FIntVector ChunkSize = FIntVector(16, 16, 16);

	UPROPERTY(EditAnywhere, Category = "Planet")
	float VoxelSize;

	UPROPERTY(EditAnywhere, Category = "Planet")
	FVector ChunkLocation;

	UPROPERTY(EditAnywhere, Category = "Planet")
	int PlanetRadius;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	FChunkMeshDataVoxel MeshData;
	FChunkMeshDataVoxel MeshData2;
	FChunkMeshDataVoxel MeshData3;
	//FChunkMeshDataVoxel MeshData4;
	//FChunkMeshDataVoxel MeshData5;

	TObjectPtr<UProceduralMeshComponent> Mesh;

	FastNoiseLite* Noise;

	TArray<FBlock> Blocks;

	FVector indexToFvector(int index);
	int FVectortoIndex(FVector pos);

	void addVertices(FChunkMeshDataVoxel& data, Direction direction, FVector location);
	void addTriangles(FChunkMeshDataVoxel& data, Shape shape);
	void addNormals(FChunkMeshDataVoxel& data, Shape shape, Direction direction);
	bool isCube(FVector position);

	FBlock getBlockGeneration(FVector position);

	TArray<Direction> rotationKeys = { UpBack, UpRight, UpForward, UpLeft, DownBack,
	DownRight, DownForward, DownLeft };

	TArray<TArray<int>> rotations = {
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 ,16, 17},
		{0, 3, 2, 1, 4, 9, 8, 7, 6, 5, 12, 11, 10, 15, 14, 13 ,16, 17},
		{0, 3, 4, 1, 2, 9, 10, 11, 12, 5, 6, 7, 8, 15, 16, 13 ,14, 17},
		{0, 1, 4, 3, 2, 5, 12, 11, 10, 9, 8, 7, 6, 13, 16, 15 ,14, 17},

		{17, 13, 16, 15, 14, 5, 12, 11, 10, 9, 8, 7, 6, 1, 4, 3, 2, 0},
		{17, 13, 14, 15, 16, 5, 6, 7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 0},
		{17, 15, 14, 13, 16, 9, 8, 7, 6, 5, 12, 11, 10, 3, 2, 1, 4, 0},
		{17, 15, 16, 13, 14, 9, 10, 11, 12, 5, 6, 7, 8, 3, 4, 1, 2, 0}
		

	};
	void GenerateMesh();

	void CreateQuad(FMask Mask, FIntVector AxisMask, FVector V1, FVector V2, FVector V3, FVector V4, const int Width, const int Height);

	bool GetBlockOpacity(FBlock block) const;

	FBlock GetBlock(FVector pos);

	bool CompareMask(FMask M1, FMask M2) const;

	int VertexCount = 0;

	
};
