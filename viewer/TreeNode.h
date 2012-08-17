enum RenderMode//��Ⱦģʽ
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

class BoundingBox//��Χ��
{
	Point3D posMin;
	Point3D posMax;
};

class MassProperty//��������
{
	Point3D posCentroid;//����
	Vector3D[3] vecPAxis;//��������
	double[3] dPMoments;//���Ծ�
	double dVolume;//���
	Tensor3D tenInertia;//��������
};

class ModelTreeNode{
	wchar_t* strNodeName; //�ڵ�����
	long lNodeId;
	wchar_t* strNodeInstanceName;//ʵ������
	wchar_t* strFilePath;//�ļ�·��
	bool bImportBrep;
	VizAttribute* pVizAttr;//��ʾ����
	ModelAttribute* pModelAttr;//ģ������

	enum NodeType{//�ڵ�����
		PRODUCT = 0;
		PART;
		BODY;
	}
	ModelTreeNode* pParentNode;//���ڵ�
	std::list<ModelTreeNode*> listChildNodes;
}

class VizAttribute{
	bool bVisible;//�ɼ���
	bool bDisplaySection;//��ʾ����
	double a,b,c,d;//����λ��
	bool bExplodeMode;//�ֽ�ģʽ
	double dTransparency;//͸����
	Color3D clrModel;//ģ����ɫ
	Color3D clrView;//��ʾ��ɫ
	RenderMode eDisplayMode;//��Ⱦģʽ
	Transform visTransf;//��ʾ�任
}

class ModelAttribute{//ģ������
	BoundingBox box;//��Χ��
	Point3D posCenter;//����
	double dMass;//����
	double dArea;//���
	MassProperty massProp;//��������
	Transform modelTransf;//ģ�ͱ任
}


