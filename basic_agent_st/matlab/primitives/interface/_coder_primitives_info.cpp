//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// _coder_primitives_info.cpp
//
// Code generation for function 'a_opt'
//

// Include files
#include "_coder_primitives_info.h"
#include "emlrt.h"
#include "tmwtypes.h"

// Function Declarations
static const mxArray *emlrtMexFcnResolvedFunctionsInfo();

// Function Definitions
static const mxArray *emlrtMexFcnResolvedFunctionsInfo()
{
  const mxArray *nameCaptureInfo;
  const char_T *data[10]{
      "789ced5add6fd3561477a7320d698540b521ed297b44da7012fab907b47cb44d28344149"
      "dab4684a53e7a6beb5afafeb6b87a608a4c103434880b44df0ce9fb1"
      "bf6148088987fd077b9b3409f1c003ce879bd8c44a2b57d7b57bcfcbcdf5a9fd3be75cf7"
      "fcce3dd7dc58eee618c771e7b8aefc9ceb8e13bd79a4377ec1d9c5a9",
      "1feb8d5f3be6969ce1c66df759fa67bd51c08a0ef6f4ee44a9217070671d23a8d414bdd4"
      "5201a70182e526a877340d28831244a038385969cfd0e280ea60d256"
      "b57fa74520484503719a48fa16ca83938378fce7e2eff821e3f1dc251e1187fef6c22fbc"
      "8811e01b40848a0ef90c160c04149df065059634f317e68b0001a203",
      "2d7a95cf99e19265b86d5e8fae9ab7083220d19a528f260d1d2b18618344331a6c42659b"
      "2f687807087a14d5cc7b614d263f266289a9783c3ecb9bd134748815"
      "5ead1152553588a00e9be00ab2fcdff3e8ffe408ff2d7dc35c63b98a55bdaa9b2b556d9b"
      "d3b96ed9b1e9d18e2f5dede86aead8d892411fef8347bc3f5df1ec7a",
      "bfd77d48dcaf206e74dccf1f320eceb1fff75f75c67f7e5ddae628e2dde5b54b34f12cf1"
      "0bcfebffefb72e7811875e4e48c25656d88f2fcb05ed4e2c83f1d2d5"
      "54a66f476104ce283b389739ade79f761e503dfaef5617441cfa4e163209bfda04727bee"
      "57fef7bade4f5df1ec7abfd77b30deedc44f2b4fbdbb4737effff4f7",
      "e51f68e25912f6bc2f2ca3c5ec2c49e515b4166b950bf9b584b29566793f2c79dfeb7b74"
      "7184ff96be5f879a89a85ffe07b6feffc315cfaef77bdd3f8f7ba7fc"
      "0f2d0f3c7ef0d7f734f12c092a0f7ce3821771e8b3154d2453f56c6601ad24d7242d5e34"
      "96ea1ce381b0f080d7faffec08ff2dbd8041a32a43d2eb0406b6fe7f",
      "e28a67d7fbbdde07f1eea67d6a79eadac312d5bcaf3daa9ea789674950f3fe61ebffb2a0"
      "dd2a1676f79574299991c5b96534952421eafb78ad037f77797ec4a1"
      "f73b0fd8f37e7527e6c807b4dea721fd68d398e0ee035ebae2d9f57eaffff0b8f7f97fd3"
      "c5cfe3ca5fdf3da6bb0ff8f76c7286269e2561e783967123393fb77b",
      "73bbd9cc2b302526925be9ca22e303c60776bf0fbbaffcbc3fd1a183c0f2c10b573cbbde"
      "eff51f1a77f31da095bf5edda7cb076f5ebc9ba7896749d8f9409a8b"
      "ddd8d1662a39a991694dd54adac6753193627c10363e60fda1a3e1b1fed070ff597f880e"
      "1eeb0f1dcff34fcbb900d1b17a82be0f6d9bd3b91ed47d4040bf0f6d",
      "c79deaf7a11f7fa3bb0f783dfef61c4d3c4bc2ce032259bf85a562767655c82d2c5776e7"
      "95993261df0985860758fd7f343c56ff0ff79fd5ff74f058fd7f3ccf"
      "3f2dfd1f7bde3f31e7c31db3d8f9b02ffb00aae7c3b4f701ef27ff9fa4896749d8f920b7"
      "322d4d43a35cdc476a7e35b5b4bedb586db2f300c6070ebf8f7e3eac",
      "62d2a783c0f241f0ce8707e24ef17c98361f4cac681768e25912763e886db4d4f47a7a67"
      "6e5a9236f2686f3e56d990d8fe20747cc0fa4347c363fda1e1feb3fe"
      "101d3cd61ff2f6fc4f2f48ab86",
      ""};
  nameCaptureInfo = nullptr;
  emlrtNameCaptureMxArrayR2016a(&data[0], 18776U, &nameCaptureInfo);
  return nameCaptureInfo;
}

mxArray *emlrtMexFcnProperties()
{
  mxArray *xEntryPoints;
  mxArray *xInputs;
  mxArray *xResult;
  const char_T *propFieldName[9]{"Version",
                                 "ResolvedFunctions",
                                 "Checksum",
                                 "EntryPoints",
                                 "CoverageInfo",
                                 "IsPolymorphic",
                                 "PropertyList",
                                 "UUID",
                                 "ClassEntryPointIsHandle"};
  const char_T *epFieldName[8]{
      "QualifiedName",    "NumberOfInputs", "NumberOfOutputs", "ConstantInputs",
      "ResolvedFilePath", "TimeStamp",      "Constructor",     "Visible"};
  xEntryPoints =
      emlrtCreateStructMatrix(1, 6, 8, (const char_T **)&epFieldName[0]);
  xInputs = emlrtCreateLogicalMatrix(1, 7);
  emlrtSetField(xEntryPoints, 0, "QualifiedName", emlrtMxCreateString("a_opt"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(7.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 0, "ResolvedFilePath",
      emlrtMxCreateString(
          "/home/fehinti/Documents/UniTrento/Semester 3/Intelligent Vehicles "
          "and Autonomous Driving/Project materials-20241117/solution/a_o"
          "pt.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(739593.76238425926));
  emlrtSetField(xEntryPoints, 0, "Constructor",
                emlrtMxCreateLogicalScalar(false));
  emlrtSetField(xEntryPoints, 0, "Visible", emlrtMxCreateLogicalScalar(true));
  xInputs = emlrtCreateLogicalMatrix(1, 7);
  emlrtSetField(xEntryPoints, 1, "QualifiedName",
                emlrtMxCreateString("pass_primitive"));
  emlrtSetField(xEntryPoints, 1, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(7.0));
  emlrtSetField(xEntryPoints, 1, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(6.0));
  emlrtSetField(xEntryPoints, 1, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 1, "ResolvedFilePath",
      emlrtMxCreateString(
          "/home/fehinti/Documents/UniTrento/Semester 3/Intelligent Vehicles "
          "and Autonomous Driving/Project materials-20241117/solution/pas"
          "s_primitive.m"));
  emlrtSetField(xEntryPoints, 1, "TimeStamp",
                emlrtMxCreateDoubleScalar(739571.44050925924));
  emlrtSetField(xEntryPoints, 1, "Constructor",
                emlrtMxCreateLogicalScalar(false));
  emlrtSetField(xEntryPoints, 1, "Visible", emlrtMxCreateLogicalScalar(true));
  xInputs = emlrtCreateLogicalMatrix(1, 5);
  emlrtSetField(xEntryPoints, 2, "QualifiedName",
                emlrtMxCreateString("pass_primitive_j0"));
  emlrtSetField(xEntryPoints, 2, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(5.0));
  emlrtSetField(xEntryPoints, 2, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(3.0));
  emlrtSetField(xEntryPoints, 2, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 2, "ResolvedFilePath",
      emlrtMxCreateString(
          "/home/fehinti/Documents/UniTrento/Semester 3/Intelligent Vehicles "
          "and Autonomous Driving/Project materials-20241117/solution/pas"
          "s_primitive_j0.m"));
  emlrtSetField(xEntryPoints, 2, "TimeStamp",
                emlrtMxCreateDoubleScalar(739584.46261574072));
  emlrtSetField(xEntryPoints, 2, "Constructor",
                emlrtMxCreateLogicalScalar(false));
  emlrtSetField(xEntryPoints, 2, "Visible", emlrtMxCreateLogicalScalar(true));
  xInputs = emlrtCreateLogicalMatrix(1, 3);
  emlrtSetField(xEntryPoints, 3, "QualifiedName",
                emlrtMxCreateString("stop_primitive"));
  emlrtSetField(xEntryPoints, 3, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(3.0));
  emlrtSetField(xEntryPoints, 3, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(3.0));
  emlrtSetField(xEntryPoints, 3, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 3, "ResolvedFilePath",
      emlrtMxCreateString(
          "/home/fehinti/Documents/UniTrento/Semester 3/Intelligent Vehicles "
          "and Autonomous Driving/Project materials-20241117/solution/sto"
          "p_primitive.m"));
  emlrtSetField(xEntryPoints, 3, "TimeStamp",
                emlrtMxCreateDoubleScalar(739584.44901620375));
  emlrtSetField(xEntryPoints, 3, "Constructor",
                emlrtMxCreateLogicalScalar(false));
  emlrtSetField(xEntryPoints, 3, "Visible", emlrtMxCreateLogicalScalar(true));
  xInputs = emlrtCreateLogicalMatrix(1, 2);
  emlrtSetField(xEntryPoints, 4, "QualifiedName",
                emlrtMxCreateString("stop_primitive_j0"));
  emlrtSetField(xEntryPoints, 4, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(2.0));
  emlrtSetField(xEntryPoints, 4, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(3.0));
  emlrtSetField(xEntryPoints, 4, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 4, "ResolvedFilePath",
      emlrtMxCreateString(
          "/home/fehinti/Documents/UniTrento/Semester 3/Intelligent Vehicles "
          "and Autonomous Driving/Project materials-20241117/solution/sto"
          "p_primitive_j0.m"));
  emlrtSetField(xEntryPoints, 4, "TimeStamp",
                emlrtMxCreateDoubleScalar(739593.979837963));
  emlrtSetField(xEntryPoints, 4, "Constructor",
                emlrtMxCreateLogicalScalar(false));
  emlrtSetField(xEntryPoints, 4, "Visible", emlrtMxCreateLogicalScalar(true));
  xInputs = emlrtCreateLogicalMatrix(1, 7);
  emlrtSetField(xEntryPoints, 5, "QualifiedName", emlrtMxCreateString("v_opt"));
  emlrtSetField(xEntryPoints, 5, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(7.0));
  emlrtSetField(xEntryPoints, 5, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 5, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 5, "ResolvedFilePath",
      emlrtMxCreateString(
          "/home/fehinti/Documents/UniTrento/Semester 3/Intelligent Vehicles "
          "and Autonomous Driving/Project materials-20241117/solution/v_o"
          "pt.m"));
  emlrtSetField(xEntryPoints, 5, "TimeStamp",
                emlrtMxCreateDoubleScalar(739593.76238425926));
  emlrtSetField(xEntryPoints, 5, "Constructor",
                emlrtMxCreateLogicalScalar(false));
  emlrtSetField(xEntryPoints, 5, "Visible", emlrtMxCreateLogicalScalar(true));
  xResult =
      emlrtCreateStructMatrix(1, 1, 9, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("24.2.0.2740171 (R2024b) Update 1"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("f25tfraOjXggJJYRSizr2F"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  return xResult;
}

// End of code generation (_coder_primitives_info.cpp)
