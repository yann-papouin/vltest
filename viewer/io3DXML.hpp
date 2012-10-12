#if !defined(Load3DXML_INCLUDE_ONCE)
#define Load3DXML_INCLUDE_ONCE

#include <vlGraphics/Geometry.hpp>
#include <vlCore/ResourceLoadWriter.hpp>
#include <vlCore/ResourceDatabase.hpp>

namespace vl
{
  class VirtualFile;
  class TextStream;
}

namespace vl
{
//-----------------------------------------------------------------------------
  ref<ResourceDatabase> load3DXML(VirtualFile* file);
  ref<ResourceDatabase> load3DXML(const String& path);
//---------------------------------------------------------------------------
// LoadWriter3DXML
//---------------------------------------------------------------------------
  /**
   * The LoadWriter3DXML class is a ResourceLoadWriter capable of reading 3DXML files.
   */
  class LoadWriter3DXML: public ResourceLoadWriter
  {
    VL_INSTRUMENT_CLASS(vl::LoadWriter3DXML, ResourceLoadWriter)

  public:
    LoadWriter3DXML(): ResourceLoadWriter("|3dxml|", "|3dxml|") {}

    ref<ResourceDatabase> loadResource(const String& path) const 
    {
      return load3DXML(path);
    }

    ref<ResourceDatabase> loadResource(VirtualFile* file) const
    {
      return load3DXML(file);
    }

    //! Not supported yet.
    bool writeResource(const String& /*path*/, ResourceDatabase* /*resource*/) const
    {
      return false;
    }

    //! Not supported yet.
    bool writeResource(VirtualFile* /*file*/, ResourceDatabase* /*resource*/) const
    {
      return false;
    }
  };
//-----------------------------------------------------------------------------
// File3DXMLLoader
//-----------------------------------------------------------------------------
  /**
   * Loads an 3DXML file.
   */
  class File3DXMLLoader
  {
  public:
    //! Loads a 3DXML file.
    ref<ResourceDatabase> load3DXML(VirtualFile* file);
    ref<ResourceDatabase> loadAscii(VirtualFile* file);
  };
};

#endif

