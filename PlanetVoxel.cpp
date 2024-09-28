// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetVoxel.h"

// Sets default values
APlanetVoxel::APlanetVoxel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	Mesh->SetCastShadow(true);
	RootComponent = Mesh;

	Noise = new FastNoiseLite();

}

// Called when the game starts or when spawned
void APlanetVoxel::BeginPlay()
{
	Super::BeginPlay();
	
	Blocks.SetNum((ChunkSize.X) * (ChunkSize.Y) * (ChunkSize.Z)); //make smaller to coincide with chunk size

	
	Blocks[2] = FBlock(CONE, DownLeft);
	Blocks[3] = FBlock(SLOPE, DownLeft);

	Noise->SetSeed(14);
	Noise->SetFrequency(0.1f / (PlanetRadius));
	Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise->SetFractalType(FastNoiseLite::FractalType_FBm);
	Noise->SetDomainWarpType(FastNoiseLite::DomainWarpType_BasicGrid);
	Noise->SetDomainWarpAmp(0.0f);
	Noise->SetFractalOctaves(3.0f);
	Noise->SetFractalLacunarity(2.0f);
	Noise->SetFractalGain(0.5f);
	

	for (int x = 0; x < ChunkSize.X; x++)
	{
		for (int y = 0; y < ChunkSize.X; y++)
		{
			for (int z = 0; z < ChunkSize.Z; z++)
			{
				FVector temp = FVector(x, y, z);
				Blocks[FVectortoIndex(temp)] = getBlockGeneration(temp);

				if (
					!isCube(temp + FVector::RightVector) ||
					!isCube(temp - FVector::RightVector) ||
					!isCube(temp + FVector::ForwardVector) ||
					!isCube(temp - FVector::ForwardVector) ||
					!isCube(temp + FVector::UpVector) ||
					!isCube(temp - FVector::UpVector)
					)
				{
					if (Blocks[FVectortoIndex(temp)].shape != null && Blocks[FVectortoIndex(temp)].shape != CUBE)
					{
						addTriangles(MeshData, Blocks[FVectortoIndex(temp)].shape);
						addVertices(MeshData, Blocks[FVectortoIndex(temp)].direction, temp);
						addNormals(MeshData, Blocks[FVectortoIndex(temp)].shape, Blocks[FVectortoIndex(temp)].direction);
					}
					
				}
			}
		}
	}

	GenerateMesh();



	Mesh->SetMaterial(0, Material);
	Mesh->CreateMeshSection(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, MeshData.UVX, MeshData.UVY, MeshData.UVZ, MeshData.Colors, TArray<FProcMeshTangent>(), true);

	Mesh->SetMaterial(1, Material2);
	Mesh->CreateMeshSection(1, MeshData2.Vertices, MeshData2.Triangles, MeshData2.Normals, MeshData2.UV0, MeshData2.UVX, MeshData2.UVY, MeshData2.UVZ, MeshData2.Colors, TArray<FProcMeshTangent>(), true);

	Mesh->SetMaterial(2, Material3);
	Mesh->CreateMeshSection(2, MeshData3.Vertices, MeshData3.Triangles, MeshData3.Normals, MeshData3.UV0, MeshData2.UVX, MeshData2.UVY, MeshData2.UVZ, MeshData2.Colors, TArray<FProcMeshTangent>(), true);

	/*Mesh->SetMaterial(3, Material4);
	Mesh->CreateMeshSection(3, MeshData4.Vertices, MeshData4.Triangles, MeshData4.Normals, MeshData4.UV0, MeshData2.UVX, MeshData2.UVY, MeshData2.UVZ, MeshData2.Colors, TArray<FProcMeshTangent>(), true);

	Mesh->SetMaterial(4, Material5);
	Mesh->CreateMeshSection(4, MeshData5.Vertices, MeshData5.Triangles, MeshData5.Normals, MeshData5.UV0, MeshData2.UVX, MeshData2.UVY, MeshData2.UVZ, MeshData2.Colors, TArray<FProcMeshTangent>(), true);*/

}

FVector APlanetVoxel::indexToFvector(int index)
{
	return FVector(
		FMath::Floor(index / (ChunkSize.Y * ChunkSize.Z)),
		FMath::Floor(index /  ChunkSize.Z % ChunkSize.Y),
		FMath::Floor(index % ChunkSize.Z)
	);

}

int APlanetVoxel::FVectortoIndex(FVector pos)
{
	return (pos.X * ChunkSize.Y * ChunkSize.Z) + (pos.Y * ChunkSize.Z) + (pos.Z);
}

void APlanetVoxel::addVertices(FChunkMeshDataVoxel& data, Direction direction, FVector location)
{
	int VoxelSizeHalf = VoxelSize / 2;

	switch (direction)
	{
	case UpBack:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		break;
	case UpRight:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		break;
	case UpForward:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		break;
	case UpLeft:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		break;
	case ForwardLeft:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));


		}
		break;
	case ForwardUp:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			
		}
		break;
	case ForwardRight:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		break;
	case ForwardDown:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		break;
	case LeftBack:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		
		break;
	case LeftUp:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			
			
			
		}
		break;
	case LeftForward:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		break;
	case LeftDown:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			}
		break;
	case BackRight:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		break;
	case BackDown:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		break;
	case BackLeft:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));	
		}
		break;
	case BackUp:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			
		}
		break;
	case RightForward:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		break;
	case RightDown:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		
		break;
	case RightBack:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		break;
	case RightUp:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		break;
	case DownBack:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		}


		break;
	case DownRight:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		break;
	case DownForward:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		break;
	case DownLeft:
		for (int i = 0; i < 4; i++)
		{
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
			data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		}
		break;
	default:
		break;
	}

	for (int i = 0; i < 32; i++)
	{
		data.Vertices[data.Vertices.Num() - (i + 1)].X += VoxelSize / 2;
		data.Vertices[data.Vertices.Num() - (i + 1)].Y += VoxelSize / 2;
		data.Vertices[data.Vertices.Num() - (i + 1)].Z += VoxelSize / 2;
	}
}

void APlanetVoxel::addTriangles(FChunkMeshDataVoxel& data, Shape shape)
{
	int v = data.Vertices.Num();
	switch (shape)
	{
	case CUBE:
		data.Triangles.Append({ 0 + v, 1 + v, 2 + v, 2 + v, 1 + v, 3 + v }); //right
		data.Triangles.Append({ 9 + v, 8 + v, 4 + v, 9 + v, 4 + v, 5 + v }); //up
		data.Triangles.Append({ 13 + v, 12 + v, 6 + v, 6 + v, 7 + v, 13 + v }); //left
		data.Triangles.Append({ 17 + v, 21 + v, 15 + v, 17 + v, 15 + v, 11 + v }); //back
		data.Triangles.Append({ 16 + v, 10 + v, 20 + v, 10 + v, 14 + v, 20 + v }); //front
		data.Triangles.Append({ 22 + v, 18 + v, 19 + v, 19 + v, 23 + v, 22 + v }); //bottom
		break;
	case SLOPE:
		data.Triangles.Append({ 6 + v, 2 + v, 3 + v, 3 + v, 7 + v, 6 + v }); //bottom 
		data.Triangles.Append({ 0 + v, 10 + v, 4 + v, 10 + v, 14 + v, 4 + v }); //front 
		data.Triangles.Append({ 8 + v, 12 + v, 15 + v, 8 + v, 15 + v, 11 + v }); //top
		data.Triangles.Append({ 16 + v, 19 + v, 18 + v }); //right 
		data.Triangles.Append({ 22 + v, 23 + v, 20 + v }); //left 
		break;
	case CORNER:
		data.Triangles.Append({ 4 + v, 7 + v, 2 + v });
		data.Triangles.Append({ 12 + v, 10 + v, 6 + v });
		data.Triangles.Append({ 15 + v, 20 + v, 14 + v });
		data.Triangles.Append({ 23 + v, 22 + v, 18 + v });
		break;
	case CORNERSLOPE:
		data.Triangles.Append({ 3 + v, 7 + v, 6 + v, 6 + v, 2 + v, 3 + v }); //bottom
		data.Triangles.Append({ 4 + v, 0 + v, 10 + v, 4 + v, 10 + v, 14 + v }); //back
		data.Triangles.Append({ 5 + v, 12 + v, 15 + v, 15 + v, 12 + v, 22 + v }); //right
		data.Triangles.Append({ 20 + v, 13 + v, 8 + v, 11 + v, 16 + v, 21 + v }); //up
		data.Triangles.Append({ 19 + v, 18 + v,24 + v }); //left
		data.Triangles.Append({ 29 + v, 23 + v, 27 + v }); //front
		break;
	case HALF:
		break;
	case CONE:
		data.Triangles.Append({ 6 + v, 2 + v, 3 + v, 3 + v, 7 + v, 6 + v }); //bottom
		data.Triangles.Append({ 11 + v, 10 + v, 4 + v}); //up
		data.Triangles.Append({ 14 + v, 15 + v, 12 + v }); //left
		data.Triangles.Append({ 19 + v, 20 + v, 23 + v}); //back
		data.Triangles.Append({ 18 + v, 22 + v, 28 + v }); //front
		break;
	default:
		break;
	}
}

void APlanetVoxel::addNormals(FChunkMeshDataVoxel& data, Shape shape, Direction direction)
{
	TArray<FVector> normals;

	switch (direction)
	{
	case UpBack:
		normals = { FVector::RightVector, FVector(-0.5,0.5,0.5).GetSafeNormal(), FVector::LeftVector, FVector::BackwardVector, FVector::ForwardVector, FVector::DownVector, FVector::UpVector, FVector(-0.5,0,0.5).GetSafeNormal(), FVector(0, 0.5, 0.5).GetSafeNormal()};
		break;
	case UpRight:
		normals = { FVector::ForwardVector, FVector(0.5,0.5,0.5).GetSafeNormal() , FVector::BackwardVector,FVector::RightVector, FVector::LeftVector, FVector::DownVector, FVector::UpVector, FVector(0,0.5,0.5).GetSafeNormal(), FVector(0.5, 0, 0.5).GetSafeNormal() };
		break;
	case UpForward:
		normals = { FVector::LeftVector, FVector(0.5,-0.5,0.5).GetSafeNormal() , FVector::RightVector, FVector::ForwardVector, FVector::BackwardVector, FVector::DownVector, FVector::UpVector, FVector(0.5,0,0.5).GetSafeNormal(), FVector(0, -0.5, 0.5).GetSafeNormal() };
		break;
	case UpLeft:
		normals = { FVector::BackwardVector, FVector(-0.5,-0.5,0.5).GetSafeNormal() , FVector::ForwardVector, FVector::LeftVector, FVector::RightVector, FVector::DownVector, FVector::UpVector, FVector(0,-0.5,0.5).GetSafeNormal(), FVector(-0.5, 0, 0.5).GetSafeNormal() };
		break;
	case ForwardLeft:
		normals = { FVector::UpVector, FVector(0.5,-0.5,0).GetSafeNormal() , FVector::DownVector, FVector::ZeroVector, FVector::RightVector, FVector::BackwardVector, FVector::DownVector, FVector(0.5,-0.5,0).GetSafeNormal(), FVector(0.5, 0, 0.5).GetSafeNormal() };
		break;
	case ForwardUp:
		normals = { FVector::ZeroVector, FVector::ZeroVector , FVector(0.5,-0.5,0), FVector::ZeroVector, FVector(0.5,0,-0.5), FVector::BackwardVector, FVector::ZeroVector, FVector::UpVector, FVector::RightVector};
		break;
	case ForwardRight:
		normals = { FVector::ZeroVector, FVector::ZeroVector , FVector::UpVector, FVector::ZeroVector, FVector::LeftVector, FVector::BackwardVector, FVector::ZeroVector, FVector(0.5,0.5,0), FVector(0.5,0,-0.5)};
		break;
	case ForwardDown:
		normals = { FVector::ZeroVector, FVector::ZeroVector , FVector(0.5,0.5,0), FVector::ZeroVector, FVector(0.5,0,0.5), FVector::BackwardVector, FVector::ZeroVector, FVector::DownVector, FVector::LeftVector};
		break;
	case LeftBack:
		normals = { FVector::UpVector, FVector::ZeroVector, FVector::DownVector, FVector::ZeroVector, FVector::ForwardVector, FVector::RightVector, FVector::DownVector, FVector(-0.5,-0.5,0).GetSafeNormal(), FVector(0, -0.5, 0.5).GetSafeNormal() };
		break;
	case LeftUp:
		normals = { FVector::ZeroVector, FVector::ZeroVector, FVector(-0.5, -0.5, 0), FVector::ZeroVector, FVector(0,-0.5,-0.5), FVector::RightVector, FVector::ZeroVector, FVector::UpVector, FVector::ForwardVector };
		break;
	case LeftForward:
		normals = { FVector::ZeroVector, FVector::ZeroVector, FVector::UpVector, FVector::ZeroVector, FVector::BackwardVector, FVector::RightVector, FVector::ZeroVector, FVector(0.5,-0.5,0), FVector(0,-0.5,-0.5)};
		break;
	case LeftDown:
		normals = { FVector::ZeroVector, FVector::ZeroVector, FVector(0.5,-0.5,0), FVector::ZeroVector, FVector(0,-0.5,0.5), FVector::RightVector, FVector::ZeroVector, FVector::DownVector, FVector::BackwardVector };
		break;
	case BackRight:
		normals = { FVector::UpVector, FVector(-0.5,0.5,0).GetSafeNormal() , FVector::DownVector, FVector::ZeroVector, FVector::LeftVector, FVector::ForwardVector, FVector::DownVector, FVector(-0.5,0.5,0).GetSafeNormal(), FVector(-0.5, 0, 0.5).GetSafeNormal() };
		break;
	case BackDown:
		normals = { FVector::ZeroVector, FVector::ZeroVector, FVector(-0.5,-0.5,0), FVector::ZeroVector, FVector(-0.5,0,0.5), FVector::ForwardVector, FVector::ZeroVector, FVector::DownVector, FVector::RightVector };
		break;
	case BackLeft:
		normals = { FVector::ZeroVector, FVector::ZeroVector, FVector::UpVector, FVector::ZeroVector, FVector::RightVector, FVector::ForwardVector, FVector::ZeroVector, FVector(-0.5,-0.5,0), FVector(-0.5,0,-0.5)};
		break;
	case BackUp:
		normals = { FVector::ZeroVector, FVector::ZeroVector, FVector(-0.5,0.5,0), FVector::ZeroVector, FVector(-0.5,0,-0.5), FVector::ForwardVector, FVector::ZeroVector, FVector::UpVector, FVector::LeftVector };
		break;
	case RightForward:
		normals = { FVector::UpVector, FVector(0.5,0.5,0).GetSafeNormal() , FVector::DownVector, FVector::ZeroVector, FVector::BackwardVector, FVector::LeftVector, FVector::DownVector, FVector(0.5,0.5,0).GetSafeNormal(), FVector(0, 0.5, 0.5).GetSafeNormal() };
		break;
	case RightDown:
		normals = { FVector::ZeroVector, FVector::ZeroVector, FVector(-0.5,0.5,0), FVector::ZeroVector, FVector(0,0.5,0.5), FVector::LeftVector, FVector::ZeroVector, FVector::DownVector, FVector::ForwardVector};
		break;
	case RightBack:
		normals = { FVector::ZeroVector, FVector::ZeroVector, FVector::UpVector, FVector::ZeroVector, FVector::ForwardVector, FVector::LeftVector, FVector::ZeroVector, FVector(-0.5,0.5,0), FVector(0,0.5,-0.5)};
		break;
	case RightUp:
		normals = { FVector::ZeroVector, FVector::ZeroVector, FVector(0.5,0.5,0), FVector::ZeroVector, FVector(0,0.5,-0.5), FVector::LeftVector, FVector::ZeroVector, FVector::UpVector, FVector::BackwardVector};
		break;
	case DownBack:
		normals = { FVector::LeftVector, FVector(-0.5,-0.5,-0.5).GetSafeNormal() , FVector::RightVector, FVector::BackwardVector, FVector::ForwardVector, FVector::UpVector, FVector::DownVector, FVector(-0.5,0,-0.5).GetSafeNormal(), FVector(0, -0.5, -0.5).GetSafeNormal() };
		break;
	case DownRight:
		normals = { FVector::BackwardVector, FVector(-0.5,0.5,-0.5).GetSafeNormal(), FVector::ForwardVector, FVector::RightVector, FVector::LeftVector, FVector::UpVector, FVector::DownVector, FVector(0,0.5,-0.5).GetSafeNormal(), FVector(-0.5, 0, -0.5).GetSafeNormal() };
		break;
	case DownForward:
		normals = { FVector::RightVector, FVector(0.5,0.5,-0.5).GetSafeNormal(), FVector::LeftVector, FVector::ForwardVector, FVector::BackwardVector, FVector::UpVector, FVector::DownVector, FVector(0.5,0,-0.5).GetSafeNormal(), FVector(0, 0.5, -0.5).GetSafeNormal() };
		break;
	case DownLeft:
		normals = { FVector::ForwardVector, FVector(0.5,-0.5,-0.5).GetSafeNormal(), FVector::BackwardVector, FVector::LeftVector, FVector::RightVector, FVector::UpVector, FVector::DownVector, FVector(0,-0.5,-0.5).GetSafeNormal(), FVector(0.5, 0, -0.5).GetSafeNormal() };
		break;
	}

	switch (shape)
	{
	case CUBE:
		data.Normals.Append({ normals[0], normals[0], normals[0], normals[0], normals[6], normals[6], normals[2], normals[2],
			normals[6], normals[6], normals[4], normals[3], normals[2], normals[2], normals[4], normals[3],
			normals[4], normals[3], normals[5], normals[5], normals[4], normals[3], normals[5], normals[5], 
			normals[0], normals[0], normals[0], normals[0], normals[0], normals[0], normals[0], normals[0]}); //6 == up
		break;
	case SLOPE:		
		data.Normals.Append({ normals[4], normals[0], normals[5], normals[5], normals[4], normals[0], normals[5], normals[5],
			normals[7], normals[0], normals[4], normals[7], normals[7], normals[0], normals[4], normals[7],
			normals[0], normals[0], normals[0], normals[0], normals[2], normals[0], normals[2], normals[2],
			normals[0], normals[0], normals[0], normals[0], normals[0], normals[0], normals[0], normals[0] });
		break;
	case CORNER:
		data.Normals.Append({ normals[0], normals[0], normals[1], normals[0], normals[1], normals[0], normals[4], normals[1],
			normals[0], normals[0], normals[4], normals[0], normals[4], normals[0], normals[2], normals[2],
			normals[0], normals[0], normals[5], normals[0], normals[2], normals[0], normals[5], normals[5],
			normals[0], normals[0], normals[0], normals[0], normals[0], normals[0], normals[0], normals[0] });
		break;
	case CORNERSLOPE:
		data.Normals.Append({ normals[4], normals[0], normals[5], normals[5], normals[4], normals[2], normals[5], normals[5],
			normals[6], normals[0], normals[4], normals[1], normals[2], normals[6], normals[4], normals[2],
			normals[1], normals[0], normals[0], normals[0], normals[6], normals[1], normals[2], normals[3],
			normals[0], normals[0], normals[0], normals[3], normals[0], normals[3], normals[0], normals[0] });
		break;
	case HALF:
		for (int k = 0; k < 32; k++)
		{
			data.Normals.Add(FVector::ForwardVector);
		}
		break;
	case null:
		for (int k = 0; k < 32; k++)
		{
			data.Normals.Add(FVector::ForwardVector);
		}
		break;
	case CONE:
		data.Normals.Append({ normals[0], normals[0], normals[5], normals[5], normals[8], normals[0], normals[5], normals[5], //right up not 1
			normals[0], normals[0], normals[8], normals[8], normals[2], normals[0], normals[2], normals[2],
			normals[0], normals[0], normals[4], normals[7], normals[7], normals[0], normals[4], normals[7],
			normals[0], normals[0], normals[0], normals[0], normals[4], normals[0], normals[0], normals[0] });
	default:
		break;
	}
}

bool APlanetVoxel::isCube(FVector position)
{
	FVector adjustPosition = position + (ChunkLocation / VoxelSize);
	FVector noisePos = adjustPosition.GetSafeNormal() * PlanetRadius;
	float radiusDistance = (PlanetRadius * 0.75) + (20 * VoxelSize * Noise->GetNoise(noisePos.X, noisePos.Y, noisePos.Z));
	radiusDistance = PlanetRadius;
	return FVector::Dist(adjustPosition, FVector::ZeroVector) < radiusDistance; //planetradius + noise(position.getnormal() * scale)
}

FBlock APlanetVoxel::getBlockGeneration(FVector position)
{
	if (isCube(position))
		return FBlock(CUBE);

	TArray<bool> nearbyBlocks = {

	isCube(position + FVector::DownVector), 
	isCube(position + FVector::DownVector + FVector::ForwardVector), 
	isCube(position + FVector::DownVector + FVector::RightVector), 
	isCube(position + FVector::DownVector + FVector::BackwardVector),
	isCube(position + FVector::DownVector + FVector::LeftVector),
	isCube(position + FVector::ForwardVector),
	isCube(position + FVector::ForwardVector + FVector::RightVector), 
	isCube(position + FVector::RightVector),
	isCube(position + FVector::RightVector + FVector::BackwardVector),
	isCube(position + FVector::BackwardVector),
	isCube(position + FVector::BackwardVector + FVector::LeftVector),
	isCube(position + FVector::LeftVector),
	isCube(position + FVector::LeftVector + FVector::ForwardVector), 
	isCube(position + FVector::UpVector + FVector::ForwardVector), 
	isCube(position + FVector::UpVector + FVector::RightVector), 
	isCube(position + FVector::UpVector + FVector::BackwardVector),
	isCube(position + FVector::UpVector + FVector::LeftVector), 
	isCube(position + FVector::UpVector)
	};

	TArray<bool> closerBlocks = {
		nearbyBlocks[0], //down
		nearbyBlocks[5], //forward
		nearbyBlocks[7], //right
		nearbyBlocks[9], //back
		nearbyBlocks[11], //left
		nearbyBlocks[17] //up
	};

	bool t = true;
	bool f = false;

	if (closerBlocks == TArray<bool>{t, t, f, f, f, f})						return FBlock(SLOPE, UpBack);
	if (closerBlocks == TArray<bool>{t, f, f, f, t, f})						return FBlock(SLOPE, UpRight);
	if (closerBlocks == TArray<bool>{t, f, f, t, f, f})						return FBlock(SLOPE, UpForward);
	if (closerBlocks == TArray<bool>{t, f, t, f, f, f})						return FBlock(SLOPE, UpLeft);
	if (closerBlocks == TArray<bool>{f, f, f, t, t, f})						return FBlock(SLOPE, RightForward);
	if (closerBlocks == TArray<bool>{f, f, t, t, f, f})						return FBlock(SLOPE, ForwardLeft);
	if (closerBlocks == TArray<bool>{f, t, t, f, f, f})						return FBlock(SLOPE, LeftBack);
	if (closerBlocks == TArray<bool>{f, t, f, f, t, f})						return FBlock(SLOPE, BackRight);
	if (closerBlocks == TArray<bool>{f, t, f, f, f, t})						return FBlock(SLOPE, DownBack);
	if (closerBlocks == TArray<bool>{f, f, f, f, t, t})						return FBlock(SLOPE, DownRight);
	if (closerBlocks == TArray<bool>{f, f, f, t, f, t})						return FBlock(SLOPE, DownForward);
	if (closerBlocks == TArray<bool>{f, f, t, f, f, t})						return FBlock(SLOPE, DownLeft);

	if (closerBlocks == TArray<bool>{t, t, f, f, t, f})						return FBlock(CORNERSLOPE, UpBack);
	if (closerBlocks == TArray<bool>{t, f, f, t, t, f})						return FBlock(CORNERSLOPE, UpRight);
	if (closerBlocks == TArray<bool>{t, f, t, t, f, f})						return FBlock(CORNERSLOPE, UpForward);
	if (closerBlocks == TArray<bool>{t, t, t, f, f, f})						return FBlock(CORNERSLOPE, UpLeft);
	if (closerBlocks == TArray<bool>{f, t, t, f, f, t})						return FBlock(CORNERSLOPE, DownBack);
	if (closerBlocks == TArray<bool>{f, t, f, f, t, t})						return FBlock(CORNERSLOPE, DownRight);
	if (closerBlocks == TArray<bool>{f, f, f, t, t, t})						return FBlock(CORNERSLOPE, DownForward);
	if (closerBlocks == TArray<bool>{f, f, t, t, f, t})						return FBlock(CORNERSLOPE, DownLeft);

	TArray<bool> rotatedNearby;

	for (int i = 0; i < rotationKeys.Num(); i++)
	{
		rotatedNearby.Empty();

		for (int k = 0; k < rotations[i].Num(); k++)
		{
			rotatedNearby.Add( nearbyBlocks[rotations[i][k]] );
		}

		if (rotatedNearby == TArray<bool>{t, t, f, f, t, f, f, f, f, f, f, f, t, f, f, f, f, f})	return FBlock(CORNER, rotationKeys[i]);
		if (rotatedNearby == TArray<bool>{t, t, t, f, t, f, f, f, f, f, f, f, t, f, f, f, f, f})	return FBlock(CORNER, rotationKeys[i]);
		if (rotatedNearby == TArray<bool>{t, t, f, t, t, f, f, f, f, f, f, f, t, f, f, f, f, f})	return FBlock(CORNER, rotationKeys[i]);
		if (rotatedNearby == TArray<bool>{t, t, t, t, t, f, f, f, f, f, f, f, t, f, f, f, f, f})	return FBlock(CORNER, rotationKeys[i]);

		if (rotatedNearby == TArray<bool>{f, t, f, f, t, f, f, f, f, f, f, f, t, f, f, f, f, f})	return FBlock(CORNER, rotationKeys[i]);
		if (rotatedNearby == TArray<bool>{f, t, f, f, t, t, f, f, f, f, f, f, t, f, f, f, f, f})	return FBlock(CORNER, rotationKeys[i]);
		if (rotatedNearby == TArray<bool>{f, t, f, f, t, t, f, f, f, f, f, f, t, t, f, f, f, f})	return FBlock(CORNER, rotationKeys[i]);
		if (rotatedNearby == TArray<bool>{f, t, f, f, t, t, t, f, f, f, f, f, t, t, f, f, f, f})	return FBlock(CORNER, rotationKeys[i]);
		if (rotatedNearby == TArray<bool>{f, t, f, f, t, f, f, f, f, f, f, t, t, f, f, f, f, f})	return FBlock(CORNER, rotationKeys[i]);
		if (rotatedNearby == TArray<bool>{f, t, f, f, t, f, f, f, f, f, f, t, t, f, f, f, t, f})	return FBlock(CORNER, rotationKeys[i]);
		if (rotatedNearby == TArray<bool>{f, t, f, f, t, f, f, f, f, f, t, t, t, f, f, f, t, f})	return FBlock(CORNER, rotationKeys[i]);

		if (rotatedNearby == TArray<bool>{f, t, f, f, t, t, t, f, f, f, f, f, t, f, f, f, f, f})	return FBlock(CORNER, rotationKeys[i]);
		if (rotatedNearby == TArray<bool>{f, t, f, f, t, f, f, f, f, f, t, t, t, f, f, f, f, f})	return FBlock(CORNER, rotationKeys[i]);
	}

	
	return null;

	
}

void APlanetVoxel::GenerateMesh()
{
	for (int Axis = 0; Axis < 3; Axis++) //take a slice equal to a plane in a perpindicular axis
	{
		const int Axis1 = (Axis + 1) % 3;
		const int Axis2 = (Axis + 2) % 3;

		const int MainAxisLimit = ChunkSize[Axis]; 
		int Axis1Limit =  ChunkSize[Axis1];
		int Axis2Limit =  ChunkSize[Axis2];

		auto DeltaAxis1 = FIntVector::ZeroValue; //growth of greedy mesh (width x Height)
		auto DeltaAxis2 = FIntVector::ZeroValue;

		auto ChunkItr = FIntVector::ZeroValue; //the blocks that we are itterating over
		auto AxisMask = FIntVector::ZeroValue; //which axis we are itterating over

		AxisMask[Axis] = 1; // [1,0,0] for x axis

		TArray<FMask> Mask;
		//TArray<FMask> MaskWater;
		Mask.SetNum(Axis1Limit * Axis2Limit); //flattened 2d array
		//MaskWater.SetNum(Axis1Limit * Axis2Limit);

		// check each slice

		for (ChunkItr[Axis] = -1; ChunkItr[Axis] < MainAxisLimit; ) //on the main axis go from -1 to the limit, slice before block 0, 1 , 2 and after 2
		{	//this "looks" in the x axis for meshes 
			int N = 0;
			//int NWater = 0;

			for (ChunkItr[Axis2] = 0; ChunkItr[Axis2] < Axis1Limit; ++ChunkItr[Axis2])
			{	//from the 2d meshes in x axis look at ones going in the y
				for (ChunkItr[Axis1] = 0; ChunkItr[Axis1] < Axis2Limit; ++ChunkItr[Axis1])
				{	//and z
					const auto CurrentBlock = GetBlock(FVector(ChunkItr)); //current
					const auto CompareBlock = GetBlock(FVector(ChunkItr + AxisMask)); //get neighbor along current iteration direction //the other side of the mask we are looking at

					bool CurrentBlockOpaque = GetBlockOpacity(CurrentBlock);
					bool CompareBlockOpaque = GetBlockOpacity(CompareBlock);

					if (CurrentBlockOpaque == CompareBlockOpaque)
					{
						/*if (CurrentBlock == EBlock::Water && !(CompareBlock == EBlock::Water) && !CompareBlockOpaque)
						{
							Mask[N++] = FMask{ EBlock::Water, 1 };
						}
						else if (CompareBlock == EBlock::Water && !(CurrentBlock == EBlock::Water) && !CurrentBlockOpaque)
						{
							Mask[N++] = FMask{ EBlock::Water, -1 };
						}
						else
						{*/
							Mask[N++] = FMask{ FBlock(null), 0 };
						//}
						//Mask[N++] = FMask{ EBlock::Null, 0 }; //both blocks are air or stone, we dont need the mesh
					}
					else if (CurrentBlockOpaque)
					{
						Mask[N++] = FMask{ CurrentBlock, 1 };
					}
					else
					{
						Mask[N++] = FMask{ CompareBlock, -1 };
					}
				}
			}

			++ChunkItr[Axis];
			N = 0;

			// Generate Mesh from the mask
			for (int j = 0; j < Axis2Limit; j++) //iterate over slice again
			{
				for (int i = 0; i < Axis1Limit; )
				{
					if (Mask[N].Normal != 0) //current mask pos needs a face
					{
						const auto CurrentMask = Mask[N];
						ChunkItr[Axis1] = i;
						ChunkItr[Axis2] = j;

						int width;

						for (width = 1; i + width < Axis1Limit && CompareMask(Mask[N + width], CurrentMask); ++width) //loop through until current width is equal to the max width
						{
						}

						int height;
						bool done = false;
						for (height = 1; j + height < Axis2Limit; ++height)
						{
							for (int k = 0; k < width; ++k)
							{
								if (CompareMask(Mask[N + k + height * Axis1Limit], CurrentMask)) continue;

								done = true;
								break;
							}
							if (done) break;
						}

						DeltaAxis1[Axis1] = width;
						DeltaAxis2[Axis2] = height;

						CreateQuad(CurrentMask, AxisMask,
							FVector(ChunkItr),//original vertex
							FVector(ChunkItr + DeltaAxis1), //+ width
							FVector(ChunkItr + DeltaAxis2), //+ height
							FVector(ChunkItr + DeltaAxis1 + DeltaAxis2), //both
							width, height);

						DeltaAxis1 = FIntVector::ZeroValue;
						DeltaAxis2 = FIntVector::ZeroValue;

						for (int l = 0; l < height; l++)
						{
							for (int k = 0; k < width; ++k)
							{
								Mask[N + k + l * Axis1Limit] = FMask{ FBlock(null), 0 };
							}
						}

						i += width;
						N += width;
					}
					else
					{
						i++;
						N++;
					}
				}
			}
		}
	}
}

void APlanetVoxel::CreateQuad(FMask Mask, FIntVector AxisMask, FVector V1, FVector V2, FVector V3, FVector V4, const int Width,
	const int Height)
{
	FChunkMeshDataVoxel* data = &MeshData2;
	int* vertex = &VertexCount;

	//if (Mask.Block == EBlock::Water) //lower waterline and set data and vertece variables
	//{
	//	V1.Z -= .2;
	//	V2.Z -= .2;
	//	V3.Z -= .2;
	//	V4.Z -= .2;
	//	data = &MeshDataWater;
	//	vertex = &VertexCountWater;
	//}

	data->Vertices.Append({ //4 vertices (corners of square)
		(V1 * VoxelSize),
		(V2 * VoxelSize),
		(V3 * VoxelSize),
		(V4 * VoxelSize)
		});

	data->Triangles.Append({ //2 triangles (3 points each) split up square
		(*vertex),
		(*vertex + 2 + Mask.Normal),
		(*vertex + 2 - Mask.Normal),
		(*vertex + 3),
		(*vertex + 1 - Mask.Normal),
		(*vertex + 1 + Mask.Normal)
		});

	FVector Normal = FVector(AxisMask * Mask.Normal);
	data->Normals.Append({ Normal, Normal, Normal, Normal });

	//const FColor Color = FColor(0, 0, 0, GetTextureIndex(Mask.Block, Normal));
	//data->Colors.Append({ Color,Color,Color,Color });

	//uv data to be used by texures (could be changed if I made triangles in same order every time
	if (Normal.X == 1 || Normal.X == -1)
	{
		data->UV0.Append({
			FVector2D(Width, Height),
			FVector2D(0, Height),
			FVector2D(Width, 0),
			FVector2D(0, 0),
			});
	}
	else // Z == 1 || Z == -1
	{
		data->UV0.Append({
			FVector2D(Height, Width),
			FVector2D(Height, 0),
			FVector2D(0, Width),
			FVector2D(0, 0),
			});
	} //y axis (top faces) right now rotations dont matter so may be wrong.

	*vertex += 4;
}

bool APlanetVoxel::GetBlockOpacity(FBlock block) const
{
	if (block.shape == CUBE)
	{
		return true;
	}
	if (block.shape == null)
	{
		return false;
	}
	return block.shape == CUBE;
}

FBlock APlanetVoxel::GetBlock(FVector pos)
{
	if (pos.X < 0 || pos.X >= ChunkSize.X || pos.Y < 0 || pos.Y >= ChunkSize.Y || pos.Z < 0 || pos.Z >= ChunkSize.Z)
		return getBlockGeneration(pos); //only works for first generate not keep generating

	int index = FVectortoIndex(pos); // = (pos.X * ChunkSize.Y * ChunkSize.Z) + (pos.Y * ChunkSize.Z) + (pos.Z);
	return Blocks[index];


}

bool APlanetVoxel::CompareMask(FMask M1, FMask M2) const
{
	return M1.Block.shape == M2.Block.shape && M1.Normal == M2.Normal;
}



