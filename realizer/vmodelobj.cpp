#include "stdafx.h"
#include "vmodelobj.h"
#include "tstream.h"
#include "tstreamreader.h"
#include "vvertexbufferformat.h"
#include "vvertexstream.h"
#include "tlog.h"
#include "cengine.h"

class VOBJVertex
{
public:
	Vector3* Vertex;
	Vector3* Normal;
	Vector3* TexCoord;

};

class VOBJFace
{
public:
	VOBJVertex Vertices[3];
};

class VOBJMaterial
{
public:
	String Name;
	Vector3 Ka;
	Vector3 Kd;
	Vector3 Ks;
	float d;
	float illum;

	String map_Ka;
	String map_Kd;

	String map_Ks;
	String map_Ns;
	String map_d;
	String map_bump;

	String bump;
	String disp;
	String decal;
};

void VModelOBJ::ReadModel( VModel* mdl, Stream* src )
{
	TStreamReader* sr = new TStreamReader(src);

	Vertices.Allocate(512);
	Normals.Allocate(512);
	TextureCoords.Allocate(512);

	Array<VOBJFace*> Faces(512);

	while(!sr->EndOfStream)
	{
		String curLine = sr->ReadLine();

		if (sr->EndOfStream)
		{
			break;
		}

		if (curLine.StartsWith("#"))
		{
			continue;
		}

		if (curLine.Length < 2)
		{
			continue;
		}

		int cmdLength;
		int prmStart;

		Seperate(curLine,cmdLength,prmStart);

		if (curLine.StartsWith("v "))
		{
			Vector3 newVertex;
			ParseVector3(curLine,prmStart,newVertex);
			Vertices.Add(newVertex);
		}
		else if (curLine.StartsWith("vn "))
		{
			Vector3 newNormal;
			ParseVector3(curLine,prmStart,newNormal);
			Normals.Add(newNormal);
		}
		else if (curLine.StartsWith("vt "))
		{
			Vector3 newTextCoord;
			ParseVector3(curLine, prmStart, newTextCoord);
			Engine.Renderer.ProcessTextureCoordinate(newTextCoord);
			TextureCoords.Add(newTextCoord);
		}
		else if (curLine.StartsWith("f "))
		{
			VOBJFace* newFace = new VOBJFace();
			ParseFace(curLine,prmStart,*newFace);
			Faces.Add(newFace);
		}
	}

	if (!ReadAsTriangleList)
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	
	
	VVertexBufferDefined< VDrawVertex >* vbo = new VVertexBufferDefined< VDrawVertex >();
	vbo->CreateDefinedBuffer(Faces.Count * 3, VertexBufferFormats->ColoredTextured1);
	vbo->ItemLength = 24;
	mdl->BaseMesh.Vertices = vbo;
	mdl->BaseMesh.SetMeshType(RL_TRIANGLELIST);

	VVertexBuilder vb;
	vb.BeginBuild(vbo);

	TArrayEnumerator<VOBJFace*> fe(Faces);
	while(fe.MoveNext())
	{
		/// WRITE FACE VERTICES TO VERTEX BUFFER OBJECT
		vb.WriteVector3(*fe.Current->Vertices[0].Vertex);
		vb.WriteUInt32(0xFFFFFFFF); // white color
		vb.WriteVector2(*fe.Current->Vertices[0].TexCoord);

		vb.WriteVector3(*fe.Current->Vertices[1].Vertex);
		vb.WriteUInt32(0xFFFFFFFF); // white color
		vb.WriteVector2(*fe.Current->Vertices[1].TexCoord);

		vb.WriteVector3(*fe.Current->Vertices[2].Vertex);
		vb.WriteUInt32(0xFFFFFFFF); // white color
		vb.WriteVector2(*fe.Current->Vertices[2].TexCoord);
	}

	vb.EndBuild();
	mdl->BaseMesh.UpdatePrimitiveCount();
}

void VModelOBJ::Seperate( const String& input, int& cmdLength, int& prmStart )
{
	bool mode = true;

	TCharacterEnumerator ce(input);
	while(ce.MoveNext())
	{
		if (mode)
		{
			if (ce.Current == ' ')
			{
				cmdLength = ce.CharIndex;
				mode = false;
			}
		}
		else
		{
			if(ce.Current != ' ')
			{
				prmStart = ce.CharIndex;
				break;
			}
		}
	}
}

void VModelOBJ::ParseVector3( const String& input, int start, Vector3& vec )
{
	// TODO: replace this shit
	sscanf_s(((char*)input.Data) + start,"%f %f %f", &(vec.x), &(vec.y), &(vec.z));
}

void VModelOBJ::ParseFace( const String& input, int start, VOBJFace& face )
{
		
	Array<String*> strArray = input.Split(' ', true);
	// TODO: implement read interrupted like thing for string? like TStringReader ?
	// TODO: or make it a TString function...

	int k = 0;
	// first one is command, we should start from second
	for (int i=1;i<strArray.Count;i++)
	{
		VOBJVertex faceVertex;
		ParseFaceVertex(*strArray[i],faceVertex);
		face.Vertices[k++] = faceVertex;
	}

	strArray.DeletePointers();
}

void VModelOBJ::ParseFaceVertex( const String& input, VOBJVertex& faceVertex )
{
	TArrayStack<int,32> indexes;

	Convert::ToInt32Split(input,'/',indexes);

	int normIndex = 1;

	if (indexes.Count > 0)
	{
		faceVertex.Vertex = &Vertices.Items[indexes[0]-1];
	}
	
	if (TextureCoords.Count > 0)
	{
		normIndex = 2;
		faceVertex.TexCoord = &TextureCoords.Items[indexes[1]-1];
	}

	if (Normals.Count > 0)
	{
		faceVertex.Normal = &Normals.Items[ indexes[normIndex]-1 ];
	}

	//Log.Output(LG_INF,input);
	//Log.Output(LG_INF,"% % %",sfi(indexes[0]),sfi(indexes[1]),sfi(indexes[2]));

}