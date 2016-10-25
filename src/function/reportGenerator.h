/****************************************************************************

 - Codename: CHER-Ob (Yale Computer Graphics Group)

 - Writers:  Weiqi Shi (weiqi.shi@yale.edu)

 - License:  GNU General Public License Usage
   Alternatively, this file may be used under the terms of the GNU General
   Public License version 3.0 as published by the Free Software Foundation
   and appearing in the file LICENSE.GPL included in the packaging of this
   file. Please review the following information to ensure the GNU General
   Public License version 3.0 requirements will be met:
   http://www.gnu.org/copyleft/gpl.html.

 - Warranty: This software is distributed WITHOUT ANY WARRANTY; without even
   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
   PURPOSE.

 - Acknowledgments: Some portions of this file are based on the example codes
   of ITK/VTK library from Kitware, QT API from Nokia. I would like to thank
   anonymous help from various software engineering communities.

   This file defines the different classes of bookmark that the user may work
   with (corresponding to the different file types that may be displayed).

*****************************************************************************/
#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include <QtScript>
#include <QPrinter>
#include <QWidget>
#include <QFileDialog>
#include <QDomDocument>
#include <QPainter>
#include <QFont>
#include <QTextDocument>
#include <QRectF>

#include <vtkExtractSelectedFrustum.h>
#include <vtkDataSet.h>

#include "../CHE/CHEInfoDialog.h"
#include "../vtkEnums.h"
#include "../visualization/vtkWidget.h"

struct CTSurfaceCornerPoint
{
	double point[4][3];
};

/**
 * This class is to save the current status of 3D object since when generate a report,
 * 3D object screenshot will change the view status. This class serves as a back-up.
 * After the screenshot, the status will be restored.
 */
class WidgetInfo3D
{
public:
	bool infoStatus;
	bool isMeasuring;
	bool isAnnotation;
	double pos[3]; // camera position
    double foc[3]; // focal point
    double angle; // view angle
    double view[3]; // view up direction
    double clip[2]; // clipping range
    double scale; // parallel scale
	bool isDirectionalLight;
	double orientation[3];
	double brightness;
	double contrast;
	RenderMode3D renderMode;
	int interpolation;
	int textureOn;
};

/**
 * This class is to maintain the object necessary information.
 * Main frame send the instances to the reportGenerate class.
 */

class ReportObject : public QWidget
{
	Q_OBJECT

public:
	WidgetMode mMode;
	QString mName;
	QString mNotesPath;
	QVector<QPair<QString, NoteType> > mNotes;	// note content and type (2D/3D)
	QVector<QString> mPictures;	// image notes
	QVector<int> mCategories;	// selected categories for notes
	QString mCHEName;
	VtkWidget* mGla;
};

/**
 * This class is to generate a report based on users choose from reportFilter dialog.
 */

class ReportGenerator : public QWidget
{
	Q_OBJECT

public:
	/**
	 * @brief  Constructor.
	 * @param  path     The absolute location of the path.
	 * @param  project  Whether the report is about a project or CHE.
	 */
	ReportGenerator(QString path, bool project = true);

	/**
	 * @brief  Set necessary information for the report if it is a project.
	 */
	void setProjectName(const QString project)	{mProjectName = project;}
	void setUserName(const QString userName)	{mUserName = userName;}
	void setKeyword(const QString keyword)	{mKeyword = keyword;}
	void setAffiliation(const QString affiliation)	{mAffiliation = affiliation;}
	void setDescription(const QString description)	{mDescription = description;}

	/**
	 * @brief  Set CHE text information for the report if it is a CHE.
	 */
	void setCHEInfo(const CHEInfoBasic* info);

	/**
	 * @brief  Provide API for main frame to add a report object.
	 * @param  object  The added object.
	 */
	void addObject(ReportObject* object)	{mObjects.push_back(object);}

	/**
	 * @brief  Start to generate report.
	 */
	void generate();

	/**
	 * @brief  Find main frame.
	 */
	static MainWindow* mw();

private:
	/**
	 * @brief  Detect whether the points can be seen from current directon.
	 *         There are six directions to generate screenshot for 3D object.
	 *         If the point is visible then transform it from world coordinate to image coordinate.
	 *         This function is used to mark note labels as id numbers on screenshot for 3D images.
	 *         It works for point note and surface note. For surface note, the center point of the surface
	 *         is calculated and tested. If the center point is visible then we mark the note label.
	 * @param  render         The current vtkRenderer that shows the object.
	 * @param  points         The vector of points to be detected in world coordinate.
	 * @param  visiblePoints  The vector of visible points in image coordinate.
	 */
	void detectPointVisibility(vtkSmartPointer<vtkRenderer> render, 
		QVector<double*> points, QVector<QPair<double, double> >& visiblePoints);

	/**
	 * @brief  Detect whether the frustum can be seen from current directon.
	 *         There are six directions to generate screenshot for 3D object.
	 *         If the point is visible then transform it from world coordinate to image coordinate.
	 *         This function is used to mark note labels as id numbers on screenshot for 3D images.
	 *         It works for frustum note. The center point of frustum is calculated and serves as the
	 *         origin point of ray tracing to the opposite of viewing direction. The intersection of
	 *         the ray and the 3D object is used to test whether the frustum is visible.
	 * @param  gla            The vtkWidget of the 3D object to get camera info.
	 * @param  center         The center point of the frustum.
	 * @param  visiblePoints  The vector of visible points in image coordinate.
	 * @param  dataset        The 3D object dataset.
	 * @param  view           The view direction.
	 */
	void detectFrustumVisibility(const VtkWidget* gla, QVector<double*> center,
		QVector<QPair<double, double> >& visiblePoints, QVector<vtkSmartPointer<vtkDataSet> > dataset, OrthogonalView3D view);


	/**
	 * @brief  Detect whether the points can be seen from current directon.
	 *         There are six directions to generate screenshot for 3D object.
	 *         If the point is visible then transform it from world coordinate to image coordinate.
	 *         This function is used to mark note labels as id numbers on screenshot for 3D images.
	 *         It works for point note and surface note. For surface note, the center point of the surface
	 *         is calculated and tested. If the center point is visible then we mark the note label.
	 * @param  render         The current vtkRenderer that shows the object.
	 * @param  points         The vector of points to be detected in world coordinate.
	 * @param  visiblePoints  The vector of visible points in image coordinate.
	 */
	void detectCTSurfaceVisibility(vtkSmartPointer<vtkRenderer> render, 
		QVector<QPair<double*, CTSurfaceCornerPoint> > points, QVector<QPair<double*, CTSurfaceCornerPoint> >& visiblePoints);

	/**
	 * @brief  Save the current vtkWidget info to prepare for the screenshot.
	 * @param  gla   The vtkWidget of the 3D object.
	 * @param  info  The saved widget info.
	 */
	void saveWidgetinfo(const VtkWidget* gla, WidgetInfo3D &info);

	/**
	 * @brief  Init the vtkWidget for the screenshot.
	 * @param  gla         The vtkWidget of the 3D or CT Volume object.
	 * @param  isCTVolume  If the widget is CT Volume.
	 */
	void initWidget(VtkWidget* gla, bool isCTVolume);

	/**
	 * @brief  recover the vtkWidget after screenshot.
	 * @param  gla         The vtkWidget of the 3D or CT Volume object.
	 * @param  info        The previous saved widget info.
	 * @param  isCTVolume  If the widget is CT Volume.
	 */
	void recoverWidget(VtkWidget* gla, WidgetInfo3D info, bool isCTVolume);

	/**
	 * @brief  Compute center point for surface note for 3D object.
	 * @param  gla      The vtkWidget of the 3D object.
	 * @param  cellIds  The selected cell ids in the surface.
	 * @param  center   The center point in world coordinate.
	 */
	void computeCenter(const VtkWidget* gla, QVector<int> cellIds, double* center);

	/**
	 * @brief  Compute center point for frustum note for 3D object.
	 * @param  gla       The vtkWidget of the 3D object.
	 * @param  points    The center points of planes that defines the frustum.
	 * @param  normals   The normal vectors of planes that defines the frustum.
	 * @param  center    The center point in world coordinate.
	 * @param  polyData  The 3D object dataset.
	 */
	void computeCenter(const VtkWidget* gla, vtkSmartPointer<vtkPoints> points,
		vtkSmartPointer<vtkDataArray> normals, double* center, vtkSmartPointer<vtkDataSet>& polyData);

	/**
	 * @brief  Compute center point for surface note for CT Volume data.
	 * @param  cornerPoints  The vector of four corner points that define the surface.
	 * @param  center        The center point in world coordinate.
	 */
	void computeCenter(CTSurfaceCornerPoint cornerPoints, double* center);

private:
	bool isProject;
	bool isPdf;
	QString mLocation;
	QPrinter* mPrinter;
	QTextDocument* mDoc;
	QString mProjectName;
	QString mUserName;
	QString mKeyword, mAffiliation, mDescription;
	CHEInfoBasic* mCHEInfo;
	QVector<ReportObject*> mObjects;

};

#endif // REPORT_GENERATOR_H