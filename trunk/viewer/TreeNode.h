enum RenderMode//渲染模式
{
	WIREFRAME;
	PHONEG_SHADED;
	GOURAUD_SHADED;
	GOURAUD_SHADED_WITH_LINES;
	FLAT_SHADED;
	HIDDEN_LINE;
	MESH;
};

class Point3D
{
	double x;
	double y;
	double z;
};

class Vector3D
{
	double x;
	double y;
	double z;
};

class Color3D
{
	double r;
	double g;
	double b;
};

class BoundingBox//包围盒
{
	Point3D posMin;
	Point3D posMax;
};

class MassProperty//质量属性
{
	Point3D posCentroid;//重心
	Vector3D[3] vecPAxis;//惯性主轴
	double[3] dPMoments;//惯性矩
	double dVolume;//体积
	Tensor3D tenInertia;//惯性张量
};

class ModelTreeNode{
	wchar_t* strNodeName; //节点名称
	long lNodeId;
	wchar_t* strNodeInstanceName;//实例名称
	wchar_t* strFilePath;//文件路径
	bool bImportBrep;
	VizAttribute* pVizAttr;//显示属性
	ModelAttribute* pModelAttr;//模型属性

	enum NodeType{//节点类型
		PRODUCT = 0;
		PART;
		BODY;
	}
	ModelTreeNode* pParentNode;//父节点
	std::list<ModelTreeNode*> listChildNodes;
}

class VizAttribute{
	bool bVisible;//可见性
	bool bDisplaySection;//显示截面
	double a,b,c,d;//截面位置
	bool bExplodeMode;//分解模式
	double dTransparency;//透明度
	Color3D clrModel;//模型颜色
	Color3D clrView;//显示颜色
	RenderMode eDisplayMode;//渲染模式
	Transform visTransf;//显示变换
}

class ModelAttribute{//模型属性
	BoundingBox box;//包围盒
	Point3D posCenter;//中心
	double dMass;//重量
	double dArea;//面积
	MassProperty massProp;//质量属性
	Transform modelTransf;//模型变换
}


