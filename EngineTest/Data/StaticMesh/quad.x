xof 0303txt 0032
template FVFData {
 <b6e70a0e-8ef9-4e83-94ad-ecc8b0c04897>
 DWORD dwFVF;
 DWORD nDWords;
 array DWORD data[nDWords];
}

template EffectInstance {
 <e331f7e4-0559-4cc2-8e99-1cec1657928f>
 STRING EffectFilename;
 [...]
}

template EffectParamFloats {
 <3014b9a0-62f5-478c-9b86-e4ac9f4e418b>
 STRING ParamName;
 DWORD nFloats;
 array FLOAT Floats[nFloats];
}

template EffectParamString {
 <1dbc4c88-94c1-46ee-9076-2c28818c9481>
 STRING ParamName;
 STRING Value;
}

template EffectParamDWord {
 <e13963bc-ae51-4c5d-b00f-cfa3a9d97ce5>
 STRING ParamName;
 DWORD Value;
}


Mesh Plane01 {
 4;
 -10.000000;0.000000;-10.000000;,
 -10.000000;0.000000;10.000000;,
 10.000000;0.000000;-10.000000;,
 10.000000;0.000000;10.000000;;
 2;
 3;0,1,2;,
 3;1,3,2;;

 MeshNormals  {
  1;
  0.000000;1.000000;0.000000;;
  2;
  3;0,0,0;,
  3;0,0,0;;
 }


 MeshMaterialList {
  1;
  1;
  0;;
   Material {
    0.8;0.8;0.8;1.000000;;
    0.000000;
    0.000000;0.000000;0.000000;;
    0.000000;0.000000;0.000000;;
    TextureFilename {
     "ground.bmp";
    }
  }
 }

 MeshTextureCoords {
  4;
  0.000000;5.000000;,
  0.000000;0.000000;,
  5.000000;5.000000;,
  5.000000;0.000000;,
 }
}