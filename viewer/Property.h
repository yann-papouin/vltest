enum RenderMode//渲染模式
{
	WIREFRAME;
	GOURAUD_SHADED;
	GOURAUD_SHADED_WITH_LINES;
	HIDDEN_LINE;
	WIREFRAME_WITH_SILHOUETTE;
	WIREFRAME_WITH_EDGES;
	PHONEG_SHADED;
	PHONEG_SHADED_WITH_LINES;
};

class Point3
{
	double x;
	double y;
	double z;
};

class Vector3
{
	double x;
	double y;
	double z;
};

class Tensor3
{

};

class Color
{
	double r,g,b,a;
};

// class MassProperty//质量属性
// {
// 	Point3 posCentroid;//重心
// 	Vector3[3] vecPAxis;//惯性主轴
// 	double[3] dPMoments;//惯性矩
// 	double dVolume;//体积
// 	Tensor3 tenInertia;//惯性张量
// };

class PropertyViz{//显示属性
	bool bVisible;//可见性
	bool bDisplaySection;//显示截面
	double a,b,c,d;//截面位置
	bool bExplodeMode;//分解模式
	double dTransparency;//透明度
	Color clrModel;//模型颜色
	Color clrView;//显示颜色
	RenderMode eDisplayMode;//渲染模式
	Matrix dVisMatrix;//显示变换
}

class PropertyModel{//模型属性
	BoundingBox box;//包围盒
	Point3 posCenter;//中心
	Matrix dModelMatrix;//模型变换
	Point3 posCentroid;//重心
	double dArea;//面积
	double dMass;//重量
	double dVolume;//体积
	Vector3[3] vecPAxis;//惯性主轴
	double[3] dPMoments;//惯性矩
	Tensor3 tenInertia;//惯性张量
}

class ModelTreeNode: public {
	wchar_t* strNodeName; //节点名称
	long lNodeId;
	wchar_t* strNodeInstanceName;//实例名称
	wchar_t* strFilePath;//文件路径
	bool bImportBrep;
	PropertyViz* pVizAttr;//显示属性
	PropertyModel* pModelAttr;//模型属性

	enum NodeType{//节点类型
		PRODUCT = 0;
		PART;
		BODY;
	}
	ModelTreeNode* pParentNode;//父节点
	std::list<ModelTreeNode*> listChildNodes;
}


