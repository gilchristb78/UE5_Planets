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

}

// Called when the game starts or when spawned
void APlanetVoxel::BeginPlay()
{
	Super::BeginPlay();
	
	Voxels.SetNum(((PlanetRadius + 2) * 2 + 1) * ((PlanetRadius + 2) * 2 + 1) * ((PlanetRadius + 2) * 2 + 1));

	Voxels[0] = 14;
	Voxels[110] = 16;

	for (int i = 0; i < Voxels.Num(); i++)
	{
		FVector temp = indexToFvector(i);
		
		if (FVector::Dist(temp, FVector::ZeroVector) < PlanetRadius)
		{
			Voxels[i] = 1;
		}
		else if (FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius && 
			FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius &&
			!(FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius)
			)
		{
			Voxels[i] = 2;
		}
		else if (FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius && 
			FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius &&
			!(FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius))
		{
			Voxels[i] = 3;
		}
		else if (FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius && 
			FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius &&
			!(FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius))
		{
			Voxels[i] = 4;
		}
		else if (FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius && 
			FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius &&
			!(FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius))
		{
			Voxels[i] = 5;
		}
		else if (FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius &&
			FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius &&
			!(FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius))
		{
			Voxels[i] = 6;
		}
		else if (FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius &&
			FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius &&
			!(FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius))
		{
			Voxels[i] = 7;
		}
		else if (FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius &&
			!(FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius))
		{
			Voxels[i] = 8;
		}
		else if (FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius &&
			!(FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius))
		{
			Voxels[i] = 9;
		}
		else if ((FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius
			|| (FVector::Dist(temp + FVector(-1, 0, -1), FVector::ZeroVector) < PlanetRadius && 
				FVector::Dist(temp + FVector(0, 1, -1), FVector::ZeroVector) < PlanetRadius))
			&&
			!(FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp + FVector(-1, 1, 0), FVector::ZeroVector) < PlanetRadius))
		{
			Voxels[i] = 10;
		}
		else if ((FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius
			|| (FVector::Dist(temp + FVector(1, 0, -1), FVector::ZeroVector) < PlanetRadius &&
				FVector::Dist(temp + FVector(0, 1, -1), FVector::ZeroVector) < PlanetRadius))
			&&
			!(FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp + FVector(1, 1, 0), FVector::ZeroVector) < PlanetRadius))
		{
			Voxels[i] = 11;
		}
		else if ((FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius
			|| (FVector::Dist(temp + FVector(1, 0, -1), FVector::ZeroVector) < PlanetRadius &&
			FVector::Dist(temp + FVector(0, -1, -1), FVector::ZeroVector) < PlanetRadius))
			&&
			!(FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp + FVector(1, -1, 0), FVector::ZeroVector) < PlanetRadius))
		{
			Voxels[i] = 12;
		}
		else if ((FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius
			|| (FVector::Dist(temp + FVector(-1, 0, -1), FVector::ZeroVector) < PlanetRadius &&
			FVector::Dist(temp + FVector(0, -1, -1), FVector::ZeroVector) < PlanetRadius))
			&&
			!(FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp + FVector(-1, -1, 0), FVector::ZeroVector) < PlanetRadius))
		{
			Voxels[i] = 13;
		}
		else if (
			!(FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius)
			)
			{
				Voxels[i] = 14;
			}
		else if (
			!(FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius)
			)
			{
				Voxels[i] = 15;
			}
		else if (
			!(FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius)
			)
			{
				Voxels[i] = 16;
			}
		else if (
			!(FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) &&
			!(FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) &&
			(FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius)
			)
			{
				Voxels[i] = 17;
			}





		if (
			!(FVector::Dist(temp + FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) ||
			!(FVector::Dist(temp - FVector(0, 1, 0), FVector::ZeroVector) < PlanetRadius) ||
			!(FVector::Dist(temp + FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) ||
			!(FVector::Dist(temp - FVector(1, 0, 0), FVector::ZeroVector) < PlanetRadius) ||
			!(FVector::Dist(temp + FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius) ||
			!(FVector::Dist(temp - FVector(0, 0, 1), FVector::ZeroVector) < PlanetRadius)
			)
		{
			if (Voxels[i] == 1)
			{
				
				int v = MeshData2.Vertices.Num();

				addVertices(MeshData2, 1, temp);

				MeshData2.Triangles.Append({ 0 + v, 1 + v, 2 + v, 2 + v, 1 + v, 3 + v }); //right
				MeshData2.Triangles.Append({ 1 + v, 0 + v, 4 + v, 1 + v, 4 + v, 5 + v }); //up
				MeshData2.Triangles.Append({ 5 + v, 4 + v, 6 + v, 6 + v, 7 + v, 5 + v }); //left
				MeshData2.Triangles.Append({ 1 + v, 5 + v, 7 + v, 1 + v, 7 + v, 3 + v }); //back
				MeshData2.Triangles.Append({ 0 + v, 2 + v, 4 + v, 2 + v, 6 + v, 4 + v }); //front
				MeshData2.Triangles.Append({ 6 + v, 2 + v, 3 + v, 3 + v, 7 + v, 6 + v }); //bottom
			}
			else if (Voxels[i] == 2)
			{
				
				int v = MeshData.Vertices.Num();
				addVertices(MeshData, 1, temp);

				MeshData.Triangles.Append({ 6 + v, 2 + v, 3 + v, 3 + v, 7 + v, 6 + v }); //bottom 
				MeshData.Triangles.Append({ 0 + v, 2 + v, 4 + v, 2 + v, 6 + v, 4 + v }); //front 
				MeshData.Triangles.Append({ 0 + v, 4 + v, 7 + v, 0 + v, 7 + v, 3 + v }); //back 
				MeshData.Triangles.Append({ 0 + v, 3 + v, 2 + v }); //right 
				MeshData.Triangles.Append({ 6 + v, 7 + v, 4 + v }); //left 

			}
			else if (Voxels[i] == 3)
			{
				
				int v = MeshData.Vertices.Num();

				addVertices(MeshData, 2, temp);

				MeshData.Triangles.Append({ 6 + v, 2 + v, 3 + v, 3 + v, 7 + v, 6 + v }); //bottom 
				MeshData.Triangles.Append({ 0 + v, 2 + v, 4 + v, 2 + v, 6 + v, 4 + v }); //front 
				MeshData.Triangles.Append({ 0 + v, 4 + v, 7 + v, 0 + v, 7 + v, 3 + v }); //back 
				MeshData.Triangles.Append({ 0 + v, 3 + v, 2 + v }); //right 
				MeshData.Triangles.Append({ 6 + v, 7 + v, 4 + v }); //left 

			}
			else if (Voxels[i] == 4)
			{
				
				int v = MeshData.Vertices.Num();

				addVertices(MeshData, 3, temp);

				MeshData.Triangles.Append({ 6 + v, 2 + v, 3 + v, 3 + v, 7 + v, 6 + v }); //bottom 
				MeshData.Triangles.Append({ 0 + v, 2 + v, 4 + v, 2 + v, 6 + v, 4 + v }); //front 
				MeshData.Triangles.Append({ 0 + v, 4 + v, 7 + v, 0 + v, 7 + v, 3 + v }); //back 
				MeshData.Triangles.Append({ 0 + v, 3 + v, 2 + v }); //right 
				MeshData.Triangles.Append({ 6 + v, 7 + v, 4 + v }); //left 

			}
			else if (Voxels[i] == 5)
			{
				
				int v = MeshData.Vertices.Num();

				addVertices(MeshData, 4, temp);

				MeshData.Triangles.Append({ 6 + v, 2 + v, 3 + v, 3 + v, 7 + v, 6 + v }); //bottom 
				MeshData.Triangles.Append({ 0 + v, 2 + v, 4 + v, 2 + v, 6 + v, 4 + v }); //front 
				MeshData.Triangles.Append({ 0 + v, 4 + v, 7 + v, 0 + v, 7 + v, 3 + v }); //back 
				MeshData.Triangles.Append({ 0 + v, 3 + v, 2 + v }); //right 
				MeshData.Triangles.Append({ 6 + v, 7 + v, 4 + v }); //left 
			}
			else if (Voxels[i] == 6)
			{
				
				int v = MeshData.Vertices.Num();

				addVertices(MeshData, 1, temp);

				MeshData.Triangles.Append({ 6 + v, 2 + v, 3 + v, 3 + v, 7 + v, 6 + v }); //bottom
				MeshData.Triangles.Append({ 1 + v, 5 + v, 7 + v, 1 + v, 7 + v, 3 + v }); //back
				MeshData.Triangles.Append({ 0 + v, 1 + v, 2 + v, 2 + v, 1 + v, 3 + v }); //right
				MeshData.Triangles.Append({ 1 + v, 0 + v, 5 + v, 6 + v, 5 + v, 0 + v }); //up
				MeshData.Triangles.Append({ 6 + v, 7 + v, 5 + v }); //left
				MeshData.Triangles.Append({ 0 + v, 2 + v, 6 + v }); //front


			}
			else if (Voxels[i] == 7)
			{
				
				int v = MeshData.Vertices.Num();
				addVertices(MeshData, 2, temp);

				MeshData.Triangles.Append({ 6 + v, 2 + v, 3 + v, 3 + v, 7 + v, 6 + v }); //bottom
				MeshData.Triangles.Append({ 1 + v, 5 + v, 7 + v, 1 + v, 7 + v, 3 + v }); //back
				MeshData.Triangles.Append({ 0 + v, 1 + v, 2 + v, 2 + v, 1 + v, 3 + v }); //right
				MeshData.Triangles.Append({ 1 + v, 0 + v, 5 + v, 6 + v, 5 + v, 0 + v }); //up
				MeshData.Triangles.Append({ 6 + v, 7 + v, 5 + v }); //left
				MeshData.Triangles.Append({ 0 + v, 2 + v, 6 + v }); //front


			}
			else if (Voxels[i] == 8)
			{
				
				int v = MeshData.Vertices.Num();

				addVertices(MeshData, 3, temp);

				MeshData.Triangles.Append({ 6 + v, 2 + v, 3 + v, 3 + v, 7 + v, 6 + v }); //bottom
				MeshData.Triangles.Append({ 1 + v, 5 + v, 7 + v, 1 + v, 7 + v, 3 + v }); //back
				MeshData.Triangles.Append({ 0 + v, 1 + v, 2 + v, 2 + v, 1 + v, 3 + v }); //right
				MeshData.Triangles.Append({ 1 + v, 0 + v, 5 + v, 6 + v, 5 + v, 0 + v }); //up
				MeshData.Triangles.Append({ 6 + v, 7 + v, 5 + v }); //left
				MeshData.Triangles.Append({ 0 + v, 2 + v, 6 + v }); //front


			}
			else if (Voxels[i] == 9)
			{
				
				int v = MeshData.Vertices.Num();

				addVertices(MeshData, 4, temp);

				MeshData.Triangles.Append({ 6 + v, 2 + v, 3 + v, 3 + v, 7 + v, 6 + v }); //bottom
				MeshData.Triangles.Append({ 1 + v, 5 + v, 7 + v, 1 + v, 7 + v, 3 + v }); //back
				MeshData.Triangles.Append({ 0 + v, 1 + v, 2 + v, 2 + v, 1 + v, 3 + v }); //right
				MeshData.Triangles.Append({ 1 + v, 0 + v, 5 + v, 6 + v, 5 + v, 0 + v }); //up
				MeshData.Triangles.Append({ 6 + v, 7 + v, 5 + v }); //left
				MeshData.Triangles.Append({ 0 + v, 2 + v, 6 + v }); //front


			}
			else if (Voxels[i] == 10)
			{
				
				int v = MeshData3.Vertices.Num();
				addVertices(MeshData3, 1, temp);

				MeshData3.Triangles.Append({ 1 + v, 2 + v, 7 + v }); //right
				MeshData3.Triangles.Append({ 1 + v, 7 + v, 3 + v }); //back
				MeshData3.Triangles.Append({ 2 + v, 1 + v, 3 + v }); //right
				MeshData3.Triangles.Append({ 2 + v, 3 + v, 7 + v }); //bottom

			}
			else if (Voxels[i] == 11)
			{
				
				int v = MeshData3.Vertices.Num();
				addVertices(MeshData3, 2, temp);

				MeshData3.Triangles.Append({ 1 + v, 2 + v, 7 + v }); //right
				MeshData3.Triangles.Append({ 1 + v, 7 + v, 3 + v }); //back
				MeshData3.Triangles.Append({ 2 + v, 1 + v, 3 + v }); //right
				MeshData3.Triangles.Append({ 2 + v, 3 + v, 7 + v }); //bottom

			}
			else if (Voxels[i] == 12)
			{
				
				int v = MeshData3.Vertices.Num();
				addVertices(MeshData3, 3, temp);

				MeshData3.Triangles.Append({ 1 + v, 2 + v, 7 + v }); //right
				MeshData3.Triangles.Append({ 1 + v, 7 + v, 3 + v }); //back
				MeshData3.Triangles.Append({ 2 + v, 1 + v, 3 + v }); //right
				MeshData3.Triangles.Append({ 2 + v, 3 + v, 7 + v }); //bottom

			}
			else if (Voxels[i] == 13)
			{
				
				int v = MeshData3.Vertices.Num();

				addVertices(MeshData3, 4, temp);

				MeshData3.Triangles.Append({ 1 + v, 2 + v, 7 + v }); //right
				MeshData3.Triangles.Append({ 1 + v, 7 + v, 3 + v }); //back
				MeshData3.Triangles.Append({ 2 + v, 1 + v, 3 + v }); //right
				MeshData3.Triangles.Append({ 2 + v, 3 + v, 7 + v }); //bottom

			}
			else if (Voxels[i] == 14)
			{
				
				int v = MeshData.Vertices.Num();
				addVertices(MeshData, 5, temp);

				MeshData.Triangles.Append({ 6 + v, 2 + v, 3 + v, 3 + v, 7 + v, 6 + v}); //bottom 
				MeshData.Triangles.Append({ 0 + v, 2 + v, 4 + v, 2 + v, 6 + v, 4 + v }); //front 
				MeshData.Triangles.Append({ 0 + v, 4 + v, 7 + v, 0 + v, 7 + v, 3 + v }); //top
				MeshData.Triangles.Append({ 0 + v, 3 + v, 2 + v }); //right 
				MeshData.Triangles.Append({ 6 + v, 7 + v, 4 + v }); //left 

			}
			else if (Voxels[i] == 15)
			{

				int v = MeshData.Vertices.Num();
				addVertices(MeshData, 6, temp);

				MeshData.Triangles.Append({ 6 + v, 2 + v, 3 + v, 3 + v, 7 + v, 6 + v }); //bottom 
				MeshData.Triangles.Append({ 0 + v, 2 + v, 4 + v, 2 + v, 6 + v, 4 + v }); //front 
				MeshData.Triangles.Append({ 0 + v, 4 + v, 7 + v, 0 + v, 7 + v, 3 + v }); //top
				MeshData.Triangles.Append({ 0 + v, 3 + v, 2 + v }); //right 
				MeshData.Triangles.Append({ 6 + v, 7 + v, 4 + v }); //left 

			}
			else if (Voxels[i] == 16)
			{

				int v = MeshData.Vertices.Num();
				addVertices(MeshData, 7, temp);

				MeshData.Triangles.Append({ 6 + v, 2 + v, 3 + v, 3 + v, 7 + v, 6 + v }); //bottom 
				MeshData.Triangles.Append({ 0 + v, 2 + v, 4 + v, 2 + v, 6 + v, 4 + v }); //front 
				MeshData.Triangles.Append({ 0 + v, 4 + v, 7 + v, 0 + v, 7 + v, 3 + v }); //top
				MeshData.Triangles.Append({ 0 + v, 3 + v, 2 + v }); //right 
				MeshData.Triangles.Append({ 6 + v, 7 + v, 4 + v }); //left 

			}
			else if (Voxels[i] == 17)
			{

				int v = MeshData.Vertices.Num();
				addVertices(MeshData, 8, temp);

				MeshData.Triangles.Append({ 6 + v, 2 + v, 3 + v, 3 + v, 7 + v, 6 + v }); //bottom 
				MeshData.Triangles.Append({ 0 + v, 2 + v, 4 + v, 2 + v, 6 + v, 4 + v }); //front 
				MeshData.Triangles.Append({ 0 + v, 4 + v, 7 + v, 0 + v, 7 + v, 3 + v }); //top
				MeshData.Triangles.Append({ 0 + v, 3 + v, 2 + v }); //right 
				MeshData.Triangles.Append({ 6 + v, 7 + v, 4 + v }); //left 

			}
		}
	}



	Mesh->SetMaterial(0, Material);
	Mesh->CreateMeshSection(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, MeshData.UVX, MeshData.UVY, MeshData.UVZ, MeshData.Colors, TArray<FProcMeshTangent>(), true);

	Mesh->SetMaterial(1, Material2);
	Mesh->CreateMeshSection(1, MeshData2.Vertices, MeshData2.Triangles, MeshData2.Normals, MeshData2.UV0, MeshData2.UVX, MeshData2.UVY, MeshData2.UVZ, MeshData2.Colors, TArray<FProcMeshTangent>(), true);

	Mesh->SetMaterial(2, Material3);
	Mesh->CreateMeshSection(2, MeshData3.Vertices, MeshData3.Triangles, MeshData3.Normals, MeshData3.UV0, MeshData2.UVX, MeshData2.UVY, MeshData2.UVZ, MeshData2.Colors, TArray<FProcMeshTangent>(), true);

}

FVector APlanetVoxel::indexToFvector(int index)
{
	int ArrayDim = ((PlanetRadius + 2) * 2 + 1);
	return FVector(
		FMath::Floor(index / (ArrayDim * ArrayDim)) - ((ArrayDim - 1) / 2),
		FMath::Floor((index % (ArrayDim * ArrayDim)) / ArrayDim) - ((ArrayDim - 1) / 2),
		(index % ArrayDim) - ((ArrayDim - 1) / 2)
	);
}

void APlanetVoxel::addVertices(FChunkMeshDataVoxel& data, int Orientation, FVector location)
{
	int VoxelSizeHalf = VoxelSize / 2;

	switch (Orientation)
	{
	case 1:
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		break;
	case 2:
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));

		break;
	case 3:
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));

		break;
	case 4:
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));

		break;
	case 5:

		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		
		if (data.Vertices.Num() == 8)
		{
			DrawDebugSphere(GetWorld(), GetActorLocation() + data.Vertices[0], 5, 3, FColor::Red, true); //2
			DrawDebugSphere(GetWorld(), GetActorLocation() + data.Vertices[1], 5, 3, FColor::Blue, true);//4
			DrawDebugSphere(GetWorld(), GetActorLocation() + data.Vertices[2], 5, 3, FColor::Green, true); //3
			DrawDebugSphere(GetWorld(), GetActorLocation() + data.Vertices[3], 5, 3, FColor::Black, true); //5
			DrawDebugSphere(GetWorld(), GetActorLocation() + data.Vertices[4], 5, 3, FColor::Magenta, true);//6
			DrawDebugSphere(GetWorld(), GetActorLocation() + data.Vertices[5], 5, 3, FColor::Purple, true); //0
			DrawDebugSphere(GetWorld(), GetActorLocation() + data.Vertices[6], 5, 3, FColor::Cyan, true);//7
			DrawDebugSphere(GetWorld(), GetActorLocation() + data.Vertices[7], 5, 3, FColor::Emerald, true);//1
		}
		
		break;
	case 6:

		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		break;
	case 7:

		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		
		break;
	case 8:

		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(-VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), -VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		data.Vertices.Add(FVector(VoxelSizeHalf + (VoxelSize * location.X), VoxelSizeHalf + (VoxelSize * location.Y), -VoxelSizeHalf + (VoxelSize * location.Z)));
		
		break;
	default:
		break;
	}


	
}



