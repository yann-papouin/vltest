enum RenderMode//��Ⱦģʽ
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

// class MassProperty//��������
// {
// 	Point3 posCentroid;//����
// 	Vector3[3] vecPAxis;//��������
// 	double[3] dPMoments;//���Ծ�
// 	double dVolume;//���
// 	Tensor3 tenInertia;//��������
// };

class PropertyViz{//��ʾ����
	bool bVisible;//�ɼ���
	bool bDisplaySection;//��ʾ����
	double a,b,c,d;//����λ��
	bool bExplodeMode;//�ֽ�ģʽ
	double dTransparency;//͸����
	Color clrModel;//ģ����ɫ
	Color clrView;//��ʾ��ɫ
	RenderMode eDisplayMode;//��Ⱦģʽ
	Matrix dVisMatrix;//��ʾ�任
}

class PropertyModel{//ģ������
	BoundingBox box;//��Χ��
	Point3 posCenter;//����
	Matrix dModelMatrix;//ģ�ͱ任
	Point3 posCentroid;//����
	double dArea;//���
	double dMass;//����
	double dVolume;//���
	Vector3[3] vecPAxis;//��������
	double[3] dPMoments;//���Ծ�
	Tensor3 tenInertia;//��������
}

class ModelTreeNode: public {
	wchar_t* strNodeName; //�ڵ�����
	long lNodeId;
	wchar_t* strNodeInstanceName;//ʵ������
	wchar_t* strFilePath;//�ļ�·��
	bool bImportBrep;
	PropertyViz* pVizAttr;//��ʾ����
	PropertyModel* pModelAttr;//ģ������

	enum NodeType{//�ڵ�����
		PRODUCT = 0;
		PART;
		BODY;
	}
	ModelTreeNode* pParentNode;//���ڵ�
	std::list<ModelTreeNode*> listChildNodes;
}


