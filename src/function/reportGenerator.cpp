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
#include <vtkSelectVisiblePoints.h>
#include <vtkInteractorObserver.h>
#include <vtkIdList.h>
#include <vtkCenterOfMass.h>
#include <vtkCellLocator.h>
#include <vtkCleanPolyData.h>
#include <vtkPlanes.h>

#include <vtkDoubleArray.h>
#include <vtkCamera.h>
#include <vtkOBBTree.h>
#include <vtkSelectPolyData.h>
#include <vtkClipPolyData.h>
#include <vtkCellArray.h>
#include <vtkMath.h>
#include <vtkStructuredPoints.h>

#include <limits>
#include <qwt_plot.h>
#include "reportGenerator.h"
#include "lightControl.h"
#include "../information/searchWidget.h"
#include "../mainWindow.h"
#include "CTControl.h"


std::string color2categoryFullName(const std::string str)
{
	if (str == "MAROON")
		return "Object / Work";
	else if (str == "RED")
		return "Physical Dimensions / Measurements"; 
	else if (str == "ORANGE")
		return "Creation";
	else if (str == "YELLOW")
		return "Materials and Techniques";
	else if (str == "LIME")
		return "Stylistic Analysis and Descriptions";
	else if (str == "GREEN")
		return "Condition and Conservation";
	else if (str == "AQUA")
		return "Analyses";
	else if (str == "BLUE")
		return "Related Works";
	else if (str == "PINK")
		return "Exhibition / Loans and Legal Issues";
	else if (str == "PURPLE")
		return "Image/Audio Documentation";
	else if (str == "WHITE")
		return "Other";
	else
		return "Other";
}

ReportGenerator::ReportGenerator(QString path, bool project)
{
	isProject = project;
	mLocation = path;
	
    mPrinter = new QPrinter(QPrinter::PrinterResolution);
    mPrinter->setOutputFormat(QPrinter::PdfFormat);
    mPrinter->setPaperSize(QPrinter::A4);
    mPrinter->setOutputFileName(mLocation);
 
    mDoc = new QTextDocument();
	QFile file(path);
	file.close();

	if (QFileInfo(mLocation).suffix() == QString("pdf"))
		isPdf = true;
	else
		isPdf = false;
}

void ReportGenerator::setCHEInfo(const CHEInfoBasic* info)
{
	mCHEInfo = new CHEInfoBasic();
	mCHEInfo->object = info->object;
	mCHEInfo->measurement = info->measurement;
	mCHEInfo->creation = info->creation;
	mCHEInfo->material = info->material;
	mCHEInfo->description = info->description;
	mCHEInfo->conservation = info->conservation;
	mCHEInfo->analyses = info->analyses;
	mCHEInfo->related = info->related;
	mCHEInfo->administration = info->administration;
	mCHEInfo->documents = info->documents;
	mCHEInfo->other = info->other;
}

void ReportGenerator::generate()
{
	QString html;
	html.append("<p><font size=\"9\" color=\"#033F81\" face=\"Times New Roman\"><div align=\"center\"><strong>\n" + mProjectName + "\n</strong></div></font></p>\n");
	html = html + QString("<p style=\"line-height:1\"><font size=\"3\" face=\"Garamond\"><div align=\"center\">\n") + mUserName + QString("\n</div></font></p>\n");
	html = html + QString("<p style=\"line-height:1\"><font size=\"3\" face=\"Garamond\"><div align=\"center\">\n") + mAffiliation + QString("\n</div></font></p>\n");
	html = html + QString("<br></br>");
	if (isProject)
	{
		html = html	+ QString("<p><font size=\"3\" color=\"#033F81\" face=\"Garamond\">\nDescription:\n</font></p>")
			+ QString("<p><font size=\"3\" face=\"Garamond\">\n") + mDescription + QString("\n</font></p><hr>\n")
			+ QString("<p><font size=\"3\" color=\"#033F81\" face=\"Garamond\">\nKeyword:\n</font></p>\n")
			+ QString("<p><font size=\"3\" face=\"Garamond\">\n") + mKeyword + QString("\n</font></p>\n");
	}
	else
	{
		html = html	+ QString("<p><font size=\"2\" color=\"#033F81\" face=\"Garamond\">\nObject / Work:</font></p>\n")
			+ QString("<p><font size=\"2\" face=\"Garamond\">") + mCHEInfo->object + QString("</font></p><hr>\n")
			+ QString("<p><font size=\"2\" color=\"#033F81\" face=\"Garamond\">\nPhysical Dimensions / Measurements:</font></p>\n")
			+ QString("<p><font size=\"2\" face=\"Garamond\">") +  mCHEInfo->measurement  + QString("</font></p><hr>\n")
			+ QString("<p><font size=\"2\" color=\"#033F81\" face=\"Garamond\">\nCreation:</font></p>\n")
			+ QString("<p><font size=\"2\" face=\"Garamond\">") + mCHEInfo->creation + QString("</font></p><hr>\n")
			+ QString("<p><font size=\"2\" color=\"#033F81\" face=\"Garamond\">\nMaterials and Techniques:</font></p>\n")
			+ QString("<p><font size=\"2\" face=\"Garamond\">") + mCHEInfo->material + QString("</font></p><hr>\n")
			+ QString("<p><font size=\"2\" color=\"#033F81\" face=\"Garamond\">\nStylistic Analysis and Descriptions:</font></p>\n")
			+ QString("<p><font size=\"2\" face=\"Garamond\">") + mCHEInfo->description + QString("</font></p><hr>\n")
			+ QString("<p><font size=\"2\" color=\"#033F81\" face=\"Garamond\">\nCondition and Conservation:</font></p>\n")
			+ QString("<p><font size=\"2\" face=\"Garamond\">") + mCHEInfo->conservation + QString("</font></p><hr>\n")
			+ QString("<p><font size=\"2\" color=\"#033F81\" face=\"Garamond\">\nAnalyses:</font></p>\n")
			+ QString("<p><font size=\"2\" face=\"Garamond\">") + mCHEInfo->analyses + QString("</font></p><hr>\n")
			+ QString("<p><font size=\"2\" color=\"#033F81\" face=\"Garamond\">\nRelated Works:</font></p>\n")
			+ QString("<p><font size=\"2\" face=\"Garamond\">") + mCHEInfo->related + QString("</font></p><hr>\n")
			+ QString("<p><font size=\"2\" color=\"#033F81\" face=\"Garamond\">\nExhibition / Loans and Legal Issues:</font></p>\n")
			+ QString("<p><font size=\"2\" face=\"Garamond\">") + mCHEInfo->administration + QString("</font></p><hr>\n")
			+ QString("<p><font size=\"2\" color=\"#033F81\" face=\"Garamond\">\nImage/Audio Documentation:</font></p>\n")
			+ QString("<p><font size=\"2\" face=\"Garamond\">") + mCHEInfo->documents + QString("</font></p><hr>\n")
			+ QString("<p><font size=\"2\" color=\"#033F81\" face=\"Garamond\">\nOthers:</font></p>\n")
			+ QString("<p><font size=\"2\" face=\"Garamond\">") + mCHEInfo->other + QString("</font></p>\n");
	}
	// create screenshot folder
	QString location = mLocation;
	location.truncate(location.lastIndexOf(QDir::separator()));
	QString tmp = location;
	tmp.append(QDir::separator() + QString("tmp"));
	QDir().mkdir(tmp);
	QDir tmpFolder(tmp);
	QString htmlFolder = location;
	if (!isPdf)
	{
		htmlFolder.append(QDir::separator() + QString("report"));
		QDir().mkdir(htmlFolder);
	}
	bool isCTModeSwitched = false;
	for (int i = 0; i < mObjects.size(); i++)
	{
		//html = html + QString("<br></br>");
		html += QString("<div style=\"page-break-after:always\"></div>");
		html = html + QString("<p><font size=\"3\" color=\"#033F81\" face=\"Garamond\"><strong>\nObject "
			+ QString::number(i+1) + QString(": ") + mObjects[i]->mName + QString("\n</strong></font></p>\n"));
		QString annotation;
		QVector<QPair<QString, NoteMode> > contents;
		QVector<QPair<int, int> > pointNote2D;
		QVector<int*> surfaceNote2D;
		QVector<double*> pointNote3D;
		QVector<QPair<double*, CTSurfaceCornerPoint> > surfaceNote3D_CT;
		QVector<double*> surfaceNote3D;
		QVector<double*> frustumNote3D;
		QVector<vtkSmartPointer<vtkDataSet> > dataset;
		if (mObjects[i]->mMode == CTSTACK)
		{
			mw()->mCtControl->setVTvolumeGPU();
			isCTModeSwitched = true;
		}
		//Parsing Notes
		for (int j = 0; j < mObjects[i]->mNotes.size(); j++)
		{
			QString note = mObjects[i]->mNotes[j].first;
			if (j == 0)
				annotation = note;
			else
			{
				int index = note.indexOf("\nColor Type:\n");
				if (index == -1)
					continue;
				QString color = note.split("\nColor Type:\n")[1].split("\n")[0];
				int type = color2type(color.toStdString());
				if (mObjects[i]->mCategories.indexOf(type) == -1)
					continue;
				NoteMode mode;
				if ((mObjects[i]->mMode == IMAGE2D || mObjects[i]->mMode == RTI2D) && mObjects[i]->mNotes[j].second == NOTE2D)
				{
					QStringList lines = note.split("\n");
					QString firstLine = lines[0];
					if (firstLine.split(" ")[0] == QString("Point"))
					{
						bool ok0, ok1;
						int x = firstLine.split(" ")[8].split(",")[0].split("(")[1].toInt(&ok0);
						int y = firstLine.split(" ")[9].split(")")[0].toInt(&ok1);
						if (!ok0 || !ok1)
						{
							qDebug() << "The Syntax of First Line is incorrect. The First Line is " << firstLine;
							continue;
						}
						pointNote2D.push_back(qMakePair(x, y));
						mode = POINTNOTE;
					}
					else if (firstLine.split(" ")[0] == QString("Surface"))
					{
						bool ok0, ok1, ok2, ok3;
						int* points = new int[4];
						points[0] = firstLine.split(" ")[11].split(",")[0].split("(")[1].toInt(&ok0);
						points[1] = firstLine.split(" ")[12].split(")")[0].toInt(&ok1);
						points[2] = firstLine.split(" ")[14].split(",")[0].split("(")[1].toInt(&ok2);
						points[3] = firstLine.split(" ")[15].split(")")[0].toInt(&ok3);
						
						if (!ok0 || !ok1 || !ok2 || !ok3)
						{
							qDebug() << "The Syntax of First Line is incorrect. The First Line is " << firstLine;
							continue;
						}
						surfaceNote2D.push_back(points);
						mode = SURFACENOTE;
					}
					else
					{
						qDebug()<<"Parsing Error in report generation: Note Type Error!"<<firstLine;
						continue;
					}
					note.remove(0, index+13);	// Remove note header for the convenience of further parsing
					contents.push_back(qMakePair(note, mode));
				}
				else if (mObjects[i]->mMode == MODEL3D && mObjects[i]->mNotes[j].second == NOTE3D)
				{
					QStringList lines = note.split("\n");
					QString firstLine = lines[0];
					if (firstLine.split(" ")[0] == QString("Point"))
					{
						bool ok0, ok1, ok2;
						double *worldPosition = new double[3];
						worldPosition[0] = firstLine.split(" ")[6].split(",")[0].split("(")[1].toDouble(&ok0);
						worldPosition[1] = firstLine.split(" ")[7].split(",")[0].toDouble(&ok1);
						worldPosition[2] = firstLine.split(" ")[8].split(")")[0].toDouble(&ok2);
						if (!ok0 || !ok1 || !ok2)
						{
							qDebug() << "The Syntax of First Line is incorrect. The First Line is " << firstLine;
						}
						//qDebug()<<"report generation"<<worldPosition[0]<<worldPosition[1]<<worldPosition[2];
						pointNote3D.push_back(worldPosition);
						mode = POINTNOTE;
					}
					else if (firstLine.split(" ")[0] == QString("Surface"))
					{
						bool ok0;
						int cellNum = firstLine.split(" ")[7].toInt(&ok0);
						if (!ok0)
						{
							qDebug() << "The Syntax of First Line is incorrect. The First Line is " << firstLine;
						}
						QTextStream in(&note);
					    while(1)
						{
							QString signal = in.readLine();
							if (signal == QString("Polygon Ids:"))
								break;
						}
						QVector<int> ids;
						for (int k = 0; k < cellNum; k++)
						{
							int cellId;
							in >> cellId;
							ids.push_back(cellId);
						}
						double *center = new double[3];
						computeCenter(mObjects[i]->mGla, ids, center);
						surfaceNote3D.push_back(center);
						mode = SURFACENOTE;
					}
					else if (firstLine.split(" ")[0] == QString("Frustum"))
					{
						QTextStream in(&note);
						vtkSmartPointer<vtkPoints> points  = vtkSmartPointer<vtkPoints>::New();
						vtkSmartPointer<vtkDoubleArray> normalArray = vtkSmartPointer<vtkDoubleArray>::New();
						vtkSmartPointer<vtkDataArray> normals = vtkDataArray::SafeDownCast(normalArray);
						in.readLine();
						for (int k = 0; k < 6; k++)
						{
							QString line = in.readLine();
							bool ok0, ok1, ok2;
							double pos[3];
							pos[0] = line.split(" ")[0].split("(")[1].split(",")[0].toDouble(&ok0);
							pos[1] = line.split(" ")[1].split(",")[0].toDouble(&ok1);
							pos[2] = line.split(" ")[2].split(")")[0].toDouble(&ok2);
							if (!ok0 || !ok1 || !ok2)
							{
								qDebug() << "The Syntax of Line is incorrect. The First Line is " << line;
							}
							points->InsertNextPoint(pos);
						}

						while(1)
						{
							QString signal = in.readLine();
							if (signal == QString("Normal Point of Each Plane:"))
								break;
						}
						normals->SetNumberOfComponents(3);
						normals->SetNumberOfTuples(6);
						for (int k = 0; k < 6; k++)
						{
							QString line = in.readLine();
							bool ok0, ok1, ok2;
							double pos[3];
							pos[0] = line.split(" ")[0].split("(")[1].split(",")[0].toDouble(&ok0);
							pos[1] = line.split(" ")[1].split(",")[0].toDouble(&ok1);
							pos[2] = line.split(" ")[2].split(")")[0].toDouble(&ok2);
							if (!ok0 || !ok1 || !ok2)
							{
								qDebug() << "The Syntax of input note is incorrect. The First Line is " << line;
							}
							normals->InsertComponent(k, 0, pos[0]);
							normals->InsertComponent(k, 1, pos[1]);
							normals->InsertComponent(k, 2, pos[2]);
						}
						
						normals->Resize(6);

						double *center = new double[3];
						vtkSmartPointer<vtkDataSet> data = vtkSmartPointer<vtkStructuredPoints>::New(); 
						computeCenter(mObjects[i]->mGla, points, normals, center, data);
						dataset.push_back(data);
						frustumNote3D.push_back(center);
						mode = FRUSTUMNOTE;
					}
					else
					{
						qDebug()<<"Parsing Error in report generation: Note Type Error!"<<firstLine;
						continue;
					}
					note.remove(0, index+13);	// Remove note header for the convenience of further parsing
					contents.push_back(qMakePair(note, mode));
				}
				else if ((mObjects[i]->mMode == CTSTACK || mObjects[i]->mMode == CTVOLUME) && mObjects[i]->mNotes[j].second == NOTE3D)
				{
					// CTSTACK will be switched to CTVOLUME and only 3D notes will be appiled.
					QStringList lines = note.split("\n");
					QString firstLine = lines[0];
					if (firstLine.split(" ")[0] == QString("Point"))
					{
						bool ok0, ok1, ok2;
						double *worldPosition = new double[3];
						worldPosition[0] = firstLine.split(" ")[6].split(",")[0].split("(")[1].toDouble(&ok0);
						worldPosition[1] = firstLine.split(" ")[7].split(",")[0].toDouble(&ok1);
						worldPosition[2] = firstLine.split(" ")[8].split(")")[0].toDouble(&ok2);
						if (!ok0 || !ok1 || !ok2)
						{
							qDebug() << "The Syntax of First Line is incorrect. The First Line is " << firstLine;
						}
						//qDebug()<<"report generation"<<worldPosition[0]<<worldPosition[1]<<worldPosition[2];
						pointNote3D.push_back(worldPosition);
						mode = POINTNOTE;
					}
					else if (firstLine.split(" ")[0] == QString("Surface"))
					{
						bool ok0, ok1, ok2;
						QTextStream in(&note);
					    while(!in.atEnd())
						{
							QString signal = in.readLine();
							if (signal == QString("Corner Points:"))
								break;
						}
						bool isParsingSuccess = true;
						CTSurfaceCornerPoint cornerPoints;
						for (int i = 0; i < 4; i++)
						{
							QString pointLine = in.readLine();
							cornerPoints.point[i][0] = pointLine.split(" ")[0].split(",")[0].split("(")[1].toDouble(&ok0);
							cornerPoints.point[i][1] = pointLine.split(" ")[1].split(",")[0].toDouble(&ok1);
							cornerPoints.point[i][2] = pointLine.split(" ")[2].split(")")[0].toDouble(&ok2);
							if (!ok0 || !ok1 || !ok2)
							{
								qDebug() << "The Syntax of First Line is incorrect. The First Line is " << firstLine;
								isParsingSuccess = false;
								break;
							}
						}
						if (!isParsingSuccess)
							continue;

						double *center = new double[3];
						computeCenter(cornerPoints, center);
						surfaceNote3D_CT.push_back(qMakePair(center, cornerPoints));
						mode = SURFACENOTE;
					}
					else
					{
						qDebug()<<"Parsing Error in report generation: Note Type Error!"<<firstLine;
						continue;
					}
					note.remove(0, index+13);	// Remove note header for the convenience of further parsing
					contents.push_back(qMakePair(note, mode));
				}
				
			}
		}
		QString screenshot = tmp;
		screenshot.append(QDir::separator() + mObjects[i]->mName);
		QString screenshotDict;
		// create images for report
		QVector<QPair<double, double> > pointNote3DFront, pointNote3DLeft, pointNote3DRight, pointNote3DTop, pointNote3DBottom, pointNote3DBack;
		QVector<QPair<double, double> > surfaceNote3DFront, surfaceNote3DLeft, surfaceNote3DRight, surfaceNote3DTop, surfaceNote3DBottom, surfaceNote3DBack;
		QVector<QPair<double*, CTSurfaceCornerPoint> > surfaceNote3DFront_CT, surfaceNote3DLeft_CT, surfaceNote3DRight_CT,
			surfaceNote3DTop_CT, surfaceNote3DBottom_CT, surfaceNote3DBack_CT;
		QVector<QPair<double, double> > frustumNote3DFront, frustumNote3DLeft, frustumNote3DRight, frustumNote3DTop, frustumNote3DBottom, frustumNote3DBack;
		WidgetInfo3D info;
		QPixmap RTIScreenShot;
		switch(mObjects[i]->mMode)
		{
			case EMPTYWIDGET:
				break;
			case IMAGE2D:
				mObjects[i]->mPictures.push_back(mObjects[i]->mGla->mFilename);
				break;
			case MODEL3D:
				saveWidgetinfo(mObjects[i]->mGla, info);
				initWidget(mObjects[i]->mGla, false);
				// generate front, left and top screenshot	
				mObjects[i]->mGla->setOrthogonalView(FRONT3D);
				screenshotDict = screenshot;
				screenshotDict.append("_front");
				mObjects[i]->mPictures.push_back(mObjects[i]->mGla->screenshot(screenshotDict));
				detectPointVisibility(mObjects[i]->mGla->mRenderer, pointNote3D, pointNote3DFront);
				detectPointVisibility(mObjects[i]->mGla->mRenderer, surfaceNote3D, surfaceNote3DFront);
				detectFrustumVisibility(mObjects[i]->mGla, frustumNote3D, frustumNote3DFront, dataset, FRONT3D);
	
				mObjects[i]->mGla->setOrthogonalView(LEFT3D);
				screenshotDict = screenshot;
				screenshotDict.append("_left");
				mObjects[i]->mPictures.push_back(mObjects[i]->mGla->screenshot(screenshotDict));
				detectPointVisibility(mObjects[i]->mGla->mRenderer, pointNote3D, pointNote3DLeft);
				detectPointVisibility(mObjects[i]->mGla->mRenderer, surfaceNote3D, surfaceNote3DLeft);
				detectFrustumVisibility(mObjects[i]->mGla, frustumNote3D, frustumNote3DLeft, dataset, LEFT3D);

				mObjects[i]->mGla->setOrthogonalView(RIGHT3D);
				screenshotDict = screenshot;
				screenshotDict.append("_right");
				mObjects[i]->mPictures.push_back(mObjects[i]->mGla->screenshot(screenshotDict));
				detectPointVisibility(mObjects[i]->mGla->mRenderer, pointNote3D, pointNote3DRight);
				detectPointVisibility(mObjects[i]->mGla->mRenderer, surfaceNote3D, surfaceNote3DRight);
				detectFrustumVisibility(mObjects[i]->mGla, frustumNote3D, frustumNote3DRight, dataset, RIGHT3D);

				mObjects[i]->mGla->setOrthogonalView(TOP3D);
				screenshotDict = screenshot;
				screenshotDict.append("_top");
				mObjects[i]->mPictures.push_back(mObjects[i]->mGla->screenshot(screenshotDict));
				detectPointVisibility(mObjects[i]->mGla->mRenderer, pointNote3D, pointNote3DTop);
				detectPointVisibility(mObjects[i]->mGla->mRenderer, surfaceNote3D, surfaceNote3DTop);
				detectFrustumVisibility(mObjects[i]->mGla, frustumNote3D, frustumNote3DTop, dataset, TOP3D);

				mObjects[i]->mGla->setOrthogonalView(BOTTOM3D);
				screenshotDict = screenshot;
				screenshotDict.append("_bottom");
				mObjects[i]->mPictures.push_back(mObjects[i]->mGla->screenshot(screenshotDict));
				detectPointVisibility(mObjects[i]->mGla->mRenderer, pointNote3D, pointNote3DBottom);
				detectPointVisibility(mObjects[i]->mGla->mRenderer, surfaceNote3D, surfaceNote3DBottom);
				detectFrustumVisibility(mObjects[i]->mGla, frustumNote3D, frustumNote3DBottom, dataset, BOTTOM3D);

				mObjects[i]->mGla->setOrthogonalView(BACK3D);
				screenshotDict = screenshot;
				screenshotDict.append("_back");
				mObjects[i]->mPictures.push_back(mObjects[i]->mGla->screenshot(screenshotDict));
				detectPointVisibility(mObjects[i]->mGla->mRenderer, pointNote3D, pointNote3DBack);
				detectPointVisibility(mObjects[i]->mGla->mRenderer, surfaceNote3D, surfaceNote3DBack);
				detectFrustumVisibility(mObjects[i]->mGla, frustumNote3D, frustumNote3DBack, dataset, BACK3D);
				recoverWidget(mObjects[i]->mGla, info, false);
				break;
			case CTSTACK:
			case CTVOLUME:
				saveWidgetinfo(mObjects[i]->mGla, info);
				initWidget(mObjects[i]->mGla, true);

				mObjects[i]->mGla->setOrthogonalView(FRONT3D);
				screenshotDict = screenshot;
				screenshotDict.append("_front");
				mObjects[i]->mPictures.push_back(mObjects[i]->mGla->screenshot(screenshotDict));
				detectPointVisibility(mObjects[i]->mGla->mRenderer, pointNote3D, pointNote3DFront);
				detectCTSurfaceVisibility(mObjects[i]->mGla->mRenderer, surfaceNote3D_CT, surfaceNote3DFront_CT);

				mObjects[i]->mGla->setOrthogonalView(LEFT3D);
				screenshotDict = screenshot;
				screenshotDict.append("_left");
				mObjects[i]->mPictures.push_back(mObjects[i]->mGla->screenshot(screenshotDict));
				detectPointVisibility(mObjects[i]->mGla->mRenderer, pointNote3D, pointNote3DLeft);
				detectCTSurfaceVisibility(mObjects[i]->mGla->mRenderer, surfaceNote3D_CT, surfaceNote3DLeft_CT);

				mObjects[i]->mGla->setOrthogonalView(RIGHT3D);
				screenshotDict = screenshot;
				screenshotDict.append("_right");
				mObjects[i]->mPictures.push_back(mObjects[i]->mGla->screenshot(screenshotDict));
				detectPointVisibility(mObjects[i]->mGla->mRenderer, pointNote3D, pointNote3DRight);
				detectCTSurfaceVisibility(mObjects[i]->mGla->mRenderer, surfaceNote3D_CT, surfaceNote3DRight_CT);

				mObjects[i]->mGla->setOrthogonalView(TOP3D);
				screenshotDict = screenshot;
				screenshotDict.append("_top");
				mObjects[i]->mPictures.push_back(mObjects[i]->mGla->screenshot(screenshotDict));
				detectPointVisibility(mObjects[i]->mGla->mRenderer, pointNote3D, pointNote3DTop);
				detectCTSurfaceVisibility(mObjects[i]->mGla->mRenderer, surfaceNote3D_CT, surfaceNote3DTop_CT);

				mObjects[i]->mGla->setOrthogonalView(BOTTOM3D);
				screenshotDict = screenshot;
				screenshotDict.append("_bottom");
				mObjects[i]->mPictures.push_back(mObjects[i]->mGla->screenshot(screenshotDict));
				detectPointVisibility(mObjects[i]->mGla->mRenderer, pointNote3D, pointNote3DBottom);
				detectCTSurfaceVisibility(mObjects[i]->mGla->mRenderer, surfaceNote3D_CT, surfaceNote3DBottom_CT);

				mObjects[i]->mGla->setOrthogonalView(BACK3D);
				screenshotDict = screenshot;
				screenshotDict.append("_back");
				mObjects[i]->mPictures.push_back(mObjects[i]->mGla->screenshot(screenshotDict));
				detectPointVisibility(mObjects[i]->mGla->mRenderer, pointNote3D, pointNote3DBack);
				detectCTSurfaceVisibility(mObjects[i]->mGla->mRenderer, surfaceNote3D_CT, surfaceNote3DBack_CT);

				recoverWidget(mObjects[i]->mGla, info, true);
				break;
			case RTI2D:
				RTIScreenShot = mObjects[i]->mGla->getRTIScreenShot();
				if (!RTIScreenShot.isNull())
				{
					screenshot.append(".png");
					RTIScreenShot.save(screenshot);
					mObjects[i]->mPictures.push_back(screenshot);
				}
				break;
			default: break;
		}

		// Switch back to origianl CT STACK mode.
		if (isCTModeSwitched)
			mw()->mCtControl->setCTStackView();

		//Process and add image
		for (int j = 0; j < mObjects[i]->mPictures.size(); j++)
		{
			QString path = QDir::toNativeSeparators(mObjects[i]->mPictures[j]);
			QImage img(path); 
			if (img == QImage())
			{
				QString message = QString("Cannot find image: ").append(mObjects[i]->mPictures[j]);
				QMessageBox::critical(this, tr("Project Error"), message);
				continue;
			}
			int imgHeight = img.height();
			int imgWidth = img.width();
			QStringList nameElements = mObjects[i]->mPictures[j].split(QDir::separator());
			QString url = nameElements[nameElements.size() - 1];
			QPainter painter(&img);
			QFont font = painter.font();
			int pointSize = font.pointSize();
			pointSize = pointSize*2 + 3.5*imgWidth / 400;
			font.setPointSize(pointSize);
			painter.setFont(font);
			painter.setRenderHint(QPainter::Antialiasing);
			painter.setPen(QPen(Qt::black, 20, Qt::DashDotLine, Qt::RoundCap));
			for (int k = 0; k < pointNote2D.size(); k++)
			{
				int x = pointNote2D[k].first, y = img.height() - pointNote2D[k].second;
				int x1 = x - pointSize/2, y1 = y - pointSize/2;
				if (x1 <= 0)
					x1 = 1;
				else if (x1 + pointSize >= imgWidth)
					x1 = imgWidth - pointSize - 1;
				if (y1 <= 0)
					y1 = 1;
				else if (y1 + pointSize >= imgHeight)
					y1 = imgHeight - pointSize - 1;

				QRectF rectangle(x1, y1, pointSize, pointSize);
				painter.setPen(QPen(Qt::red, 4));
				painter.drawRect(rectangle);
				painter.setPen(QPen(Qt::black, 4));
				painter.drawText(rectangle,  Qt::AlignCenter, QString::number(k+1));
			}
			
			for (int k = 0; k < surfaceNote2D.size(); k++)
			{
				int x1 = surfaceNote2D[k][0];
				int y1 = surfaceNote2D[k][1];
				int x2 = surfaceNote2D[k][2];
				int y2 = surfaceNote2D[k][3];
				int width = abs(x2 - x1);
				int height = abs(y2 - y1);
				y1 = img.height() - y1;
				QRectF rectangle(x1, y1, width, height);
				painter.setPen(QPen(Qt::red, 4));
				painter.drawRect(rectangle);
				painter.setPen(QPen(Qt::black, 4));
				painter.drawText(rectangle,  Qt::AlignCenter, QString::number(k+pointNote2D.size()+1));
			}
			font = painter.font();
			pointSize = pointSize * 1.2;
			//if (pointSize == 0)	pointSize = 1;
			font.setPointSize(pointSize);
			painter.setFont(font);
			QVector<QPair<double, double> > pointNote3DSelected, surfaceNote3DSelected, frustumNote3DSelected;
			QVector<QPair<double*, CTSurfaceCornerPoint> > surfaceNote3DSelected_CT;
			switch(j)
			{
				case 0: 
					pointNote3DSelected = pointNote3DFront;
					surfaceNote3DSelected = surfaceNote3DFront;
					frustumNote3DSelected = frustumNote3DFront;
					surfaceNote3DSelected_CT = surfaceNote3DFront_CT;
					break;
				case 1: 
					pointNote3DSelected = pointNote3DLeft;
					surfaceNote3DSelected = surfaceNote3DLeft;
					frustumNote3DSelected = frustumNote3DLeft;
					surfaceNote3DSelected_CT = surfaceNote3DLeft_CT;
					break;
				case 2: 
					pointNote3DSelected = pointNote3DRight;
					surfaceNote3DSelected = surfaceNote3DRight;
					frustumNote3DSelected = frustumNote3DRight;
					surfaceNote3DSelected_CT = surfaceNote3DRight_CT;
					break;
				case 3: 
					pointNote3DSelected = pointNote3DTop;
					surfaceNote3DSelected = surfaceNote3DTop;
					frustumNote3DSelected = frustumNote3DTop;
					surfaceNote3DSelected_CT = surfaceNote3DTop_CT;
					break;
				case 4: 
					pointNote3DSelected = pointNote3DBottom;
					surfaceNote3DSelected = surfaceNote3DBottom;
					frustumNote3DSelected = frustumNote3DBottom;
					surfaceNote3DSelected_CT = surfaceNote3DBottom_CT;
					break;
				case 5: 
					pointNote3DSelected = pointNote3DBack;
					surfaceNote3DSelected = surfaceNote3DBack;
					frustumNote3DSelected = frustumNote3DBack;
					surfaceNote3DSelected_CT = surfaceNote3DBack_CT;
					break;
				default:break;
			}

			for (int k = 0; k < pointNote3DSelected.size(); k++)
			{
				double x = pointNote3DSelected[k].first, y = img.height() - pointNote3DSelected[k].second;
				if (x == -1 || y == -1)
					continue;
				double x1 = x - pointSize/2, y1 = y - pointSize/2;
				if (x1 <= 0)
					x1 = 1;
				else if (x1 + pointSize >= imgWidth)
					x1 = imgWidth - pointSize - 1;
				if (y1 <= 0)
					y1 = 1;
				else if (y1 + pointSize >= imgHeight)
					y1 = imgHeight - pointSize - 1;

				QRectF rectangle(x1, y1, pointSize, pointSize);
				painter.setPen(QPen(Qt::red, 4));
				painter.drawRect(rectangle);
				painter.setPen(QPen(Qt::black, 4));
				painter.drawText(rectangle,  Qt::AlignCenter, QString::number(k+1));
			}
			for (int k = 0; k < surfaceNote3DSelected.size(); k++)
			{
				double x = surfaceNote3DSelected[k].first, y = img.height() - surfaceNote3DSelected[k].second;
				if (x == -1 || y == -1)
					continue;
				double x1 = x - pointSize/2, y1 = y - pointSize/2;
				if (x1 <= 0)
					x1 = 1;
				else if (x1 + pointSize >= imgWidth)
					x1 = imgWidth - pointSize - 1;
				if (y1 <= 0)
					y1 = 1;
				else if (y1 + pointSize >= imgHeight)
					y1 = imgHeight - pointSize - 1;

				QRectF rectangle(x1, y1, pointSize, pointSize);
				painter.setPen(QPen(Qt::red, 4));
				painter.drawRect(rectangle);
				painter.setPen(QPen(Qt::black, 4));
				painter.drawText(rectangle, Qt::AlignCenter, QString::number(k+1+pointNote3DSelected.size()));
			}
			for (int k = 0; k < frustumNote3DSelected.size(); k++)
			{
				double x = frustumNote3DSelected[k].first, y = img.height() - frustumNote3DSelected[k].second;
				if (x == -1 || y == -1)
					continue;
				double x1 = x - pointSize/2, y1 = y - pointSize/2;
				if (x1 <= 0)
					x1 = 1;
				else if (x1 + pointSize >= imgWidth)
					x1 = imgWidth - pointSize - 1;
				if (y1 <= 0)
					y1 = 1;
				else if (y1 + pointSize >= imgHeight)
					y1 = imgHeight - pointSize - 1;

				QRectF rectangle(x1, y1, pointSize, pointSize);
				painter.setPen(QPen(Qt::red, 4));
				painter.drawRect(rectangle);
				painter.setPen(QPen(Qt::black, 4));
				painter.drawText(rectangle,  Qt::AlignCenter, QString::number(k+1+pointNote3DSelected.size()+surfaceNote3DSelected.size()));
			}
			for (int k = 0; k < surfaceNote3DSelected_CT.size(); k++)
			{
				double x = surfaceNote3DSelected_CT[k].first[0], y = img.height() - surfaceNote3DSelected_CT[k].first[1];
				if (x == -1 || y == -1)
					continue;
				double x1 = x - pointSize/2, y1 = y - pointSize/2;
				if (x1 <= 0)
					x1 = 1;
				else if (x1 + pointSize >= imgWidth)
					x1 = imgWidth - pointSize - 1;
				if (y1 <= 0)
					y1 = 1;
				else if (y1 + pointSize >= imgHeight)
					y1 = imgHeight - pointSize - 1;


				QPolygon polygon;
				polygon << QPoint(surfaceNote3DSelected_CT[k].second.point[0][0], img.height() - surfaceNote3DSelected_CT[k].second.point[0][1])
					<< QPoint(surfaceNote3DSelected_CT[k].second.point[1][0], img.height() - surfaceNote3DSelected_CT[k].second.point[1][1])
					<< QPoint(surfaceNote3DSelected_CT[k].second.point[2][0], img.height() - surfaceNote3DSelected_CT[k].second.point[2][1])
					<< QPoint(surfaceNote3DSelected_CT[k].second.point[3][0], img.height() - surfaceNote3DSelected_CT[k].second.point[3][1])
					<< QPoint(surfaceNote3DSelected_CT[k].second.point[0][0], img.height() - surfaceNote3DSelected_CT[k].second.point[0][1]);

				qDebug()<<"point 0"<<surfaceNote3DSelected_CT[k].second.point[0][0]<<surfaceNote3DSelected_CT[k].second.point[0][1];
				qDebug()<<"point 1"<<surfaceNote3DSelected_CT[k].second.point[1][0]<<surfaceNote3DSelected_CT[k].second.point[1][1];
				qDebug()<<"point 2"<<surfaceNote3DSelected_CT[k].second.point[2][0]<<surfaceNote3DSelected_CT[k].second.point[2][1];
				qDebug()<<"point 3"<<surfaceNote3DSelected_CT[k].second.point[3][0]<<surfaceNote3DSelected_CT[k].second.point[3][1];

				QRectF rectangle(x1, y1, pointSize, pointSize);
				painter.setPen(QPen(Qt::red, 4));
				painter.drawPolygon(polygon);
				painter.setPen(QPen(Qt::black, 4));
				painter.drawText(rectangle, Qt::AlignCenter, QString::number(k+1+pointNote3DSelected.size()));
			}
			
			painter.end(); 
		    mDoc->addResource(QTextDocument::ImageResource, QUrl(url), img);
			
			if (mObjects[i]->mMode == IMAGE2D || mObjects[i]->mMode == RTI2D)
			{
				double height = 200, width = 300;
				height = (double)width * imgHeight / imgWidth;
				if (isPdf)
					html = html + QString("<p><div align=\"center\"><img src=\"" + url + "\"width=\"" + QString::number(width) + "\" height=\"" + QString::number(height) + "\"></div></p>\n");
				else
				{
					QString fileName = htmlFolder;
					fileName.append(QDir::separator() + url);
					img.save(fileName);
					html = html + QString("<p><div align=\"center\"><img src=\"report/" + url + "\"width=\"" + QString::number(width) + "\" height=\"" + QString::number(height) + "\"></div></p>\n");
				}
			}
			else if (mObjects[i]->mMode == MODEL3D || mObjects[i]->mMode == CTSTACK || mObjects[i]->mMode == CTVOLUME)
			{
				double height = 100, width = 150;
				height = (double)width * imgHeight / imgWidth;
				if (isPdf)
				{
					if (j == 0)
						html += QString("<p><div align=\"center\">");
					html += QString("<img src=\"" + url + "\"width=\"" + QString::number(width) + "\" height=\"" + QString::number(height) + "\">");
					if (j == mObjects[i]->mPictures.size() - 1)
						html += QString("</div></p>\n");
				}
				else
				{
					QString fileName = htmlFolder;
					fileName.append(QDir::separator() + url);
					img.save(fileName);
					if (j == 0)
						html += QString("<p><div align=\"center\">");
					html += QString("<img src=\"report/" + url + "\"width=\"" + QString::number(width) + "\" height=\"" + QString::number(height) + "\">");
					if (j == mObjects[i]->mPictures.size() - 1)
						html += QString("</div></p>\n");
				}
			}
		}
		//Add annotation and notes
		if (!annotation.isEmpty())
			html = html + QString("<p><font size=\"2\" face=\"Garamond\">") + annotation + QString("</font></p><hr>\n");
		for (int j = 0; j < contents.size(); j++)
		{
			QString content = contents[j].first;
			QString color = content.split("\nNote Start:\n")[0];
			int type = color2type(color.toStdString());
			if (mObjects[i]->mCategories.indexOf(type) == -1)
				continue;
			html += QString("<p><font size=\"2\" color=\"#033F81\" face=\"Garamond\">\n");
			switch(contents[j].second)
			{
				case POINTNOTE:		html += QString("Point Note "); break;
				case SURFACENOTE:	html += QString("Surface Note "); break;
				case FRUSTUMNOTE:	html += QString("Frustum Note "); break;
				default: continue;
			}

			QString text = content.split("\nLinked Images:\n")[0].split("Note Start:\n")[1];
			QString category = QString(color2categoryFullName(color.toStdString()).c_str());
			QStringList imagePaths = content.split("\nLinked Images:\n")[1].split("\n", QString::SkipEmptyParts);
			QDir dir(mObjects[i]->mNotesPath);
			html += QString::number(j+1) + QString(": </font></p>\n")
				+ QString("<p><font size=\"2\" color=\"#033F81\" face=\"Garamond\">Category: ") + category + QString("</font></p>");
			for (int k = 0; k < imagePaths.size(); k++)
			{
				QImage imgNote(dir.absoluteFilePath(imagePaths[k]));
				int imgNoteHeight = imgNote.height();
				int imgNoteWidth = imgNote.width();
				if (imgNoteHeight == 0 || imgNoteWidth == 0)
					continue;
				int height, width;
				if (imgNoteWidth/imgNoteHeight >= 3)
				{	
					width = 300;
					height = (double)width * imgNoteHeight / imgNoteWidth;
				}
				else if (imgNoteWidth/imgNoteHeight >= 0.5)
				{
					width = 150;
					height = (double)width * imgNoteHeight / imgNoteWidth;
				}
				else
				{
					height = 250;
					width = (double)height * imgNoteWidth / imgNoteHeight;
				}
				QString name = mObjects[i]->mName;
				name.insert(name.lastIndexOf("."), QString("_" + QString::number(j) + "_" + QString::number(k)));
				mDoc->addResource(QTextDocument::ImageResource, QUrl(name), imgNote);
				if (isPdf)
					html += QString("<p><div align=\"center\"><img src=\"" + name + "\"width=\"" + QString::number(width) + "\" height=\"" + QString::number(height) + "\"></div></p>");
				else
				{
					QString fileName = htmlFolder;
					fileName.append(QDir::separator() + name);
					imgNote.save(fileName);
					html += QString("<p><div align=\"center\"><img src=\"report/" + name + "\"width=\"" + QString::number(width) + "\" height=\"" + QString::number(height) + "\"></div></p>");
				}
			}
			html += QString("<p><font size=\"2\" face=\"Garamond\">") + text + QString("</font></p>");
			if (j != contents.size()-1)
				html += QString("<hr>\n");
		}
		for (int j = 0; j < surfaceNote2D.size(); j++)
		{
			delete surfaceNote2D[j];
		}
		for (int j = 0; j < pointNote3D.size(); j++)
		{
			delete pointNote3D[j];
		}
		for (int j = 0; j < surfaceNote3D.size(); j++)
		{
			delete surfaceNote3D[j];
		}
		for (int j = 0; j < frustumNote3D.size(); j++)
		{
			delete frustumNote3D[j];
		}
	}

	tmpFolder.setNameFilters(QStringList() << "*.*");
	tmpFolder.setFilter(QDir::Files);
	foreach(QString dirFile, tmpFolder.entryList())
	{
		tmpFolder.remove(dirFile);
	}
	QDir().rmdir(tmp);

	if (QFileInfo(mLocation).suffix() == QString("html"))
	{
		QFile file(mLocation);
		  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			qDebug() << "Cannot write to file " << mLocation; 
			return;
		}
		QTextStream out(&file);
		out << html;
		file.close();
		return;
	}

	mDoc->setHtml(html);
	mPrinter->setPageMargins(10, 10, 5, 20, QPrinter::Millimeter);
	mDoc->setPageSize(mPrinter->pageRect().size());

	QRect innerRect = mPrinter->pageRect();
    innerRect.setTop(innerRect.top());
    innerRect.setBottom(innerRect.bottom());
    QRect contentRect = QRect(QPoint(0,0), mDoc->size().toSize());
    QRect currentRect = QRect(QPoint(0,0), innerRect.size());
	

    QPainter painter(mPrinter);
	QDate date = QDate::currentDate();
	
    int count = 0;
    painter.save();
    painter.translate(0, 20);
    while (currentRect.intersects(contentRect)) 
	{
        mDoc->drawContents(&painter, currentRect);
        count++;
        currentRect.translate(0, currentRect.height());
        painter.restore();
		painter.setPen(QColor(3,63,129));
		painter.setFont(QFont("Garamond", 12, QFont::Bold));
        painter.drawText(mPrinter->pageRect().right() - 100, 10, QString("CHER-Ob"));
		painter.setPen(Qt::black);
		painter.setFont(QFont("Garamond", 8));
		painter.drawText(mPrinter->pageRect().left(), mPrinter->pageRect().bottom()+12, date.toString("MM/dd/yyyy"));
        painter.drawText(mPrinter->pageRect().right() - 70, mPrinter->pageRect().bottom()+12, QString("Page %1").arg(count));
        painter.save();
        painter.translate(0, -currentRect.height() * count+20);
        if (currentRect.intersects(contentRect))
            mPrinter->newPage();
    }
    painter.restore();
    painter.end();
}

void ReportGenerator::detectPointVisibility(vtkSmartPointer<vtkRenderer> render, QVector<double*> points, QVector<QPair<double, double> >& visiblePoints)
{
	for (int i = 0; i < points.size(); i++)
	{
		vtkSmartPointer<vtkPoints> point = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkPolyData> pointData = vtkSmartPointer<vtkPolyData>::New();
		point->SetNumberOfPoints(1);
		point->SetPoint(0, points[i][0], points[i][1], points[i][2]);
		pointData->SetPoints(point);
		vtkSmartPointer<vtkSelectVisiblePoints> selectVisiblePoints = vtkSmartPointer<vtkSelectVisiblePoints>::New();
		selectVisiblePoints->SetInput(pointData);
		selectVisiblePoints->SetRenderer(render);
		selectVisiblePoints->Update();
		if (selectVisiblePoints->GetOutput()->GetNumberOfPoints() != 0)
		{
			double *displayPt = new double[3];
			vtkInteractorObserver::ComputeWorldToDisplay (render, points[i][0], points[i][1], points[i][2], displayPt);
			visiblePoints.push_back(qMakePair(displayPt[0], displayPt[1]));
			delete displayPt;
		}
		else
		{
			double x = -1;
			visiblePoints.push_back(qMakePair(x, x));
		}
	}
}

void ReportGenerator::detectFrustumVisibility(const VtkWidget* gla, QVector<double*> center, 
											  QVector<QPair<double, double> >& visiblePoints,
											  QVector<vtkSmartPointer<vtkDataSet> > dataset,
											  OrthogonalView3D view)
{
	vtkSmartPointer<vtkCamera> camera = gla->mRenderer->GetActiveCamera();
	double cameraPos[3];
	camera->GetPosition(cameraPos);
	double point[3];
	for (int i = 0; i < center.size(); i++)
	{
		vtkSmartPointer<vtkPoints> selectionPoints = vtkSmartPointer<vtkPoints>::New();
		selectionPoints->SetNumberOfPoints(dataset[i]->GetNumberOfPoints());
		for (int j = 0; j < dataset[i]->GetNumberOfPoints(); j++)
		{
			dataset[i]->GetPoint(j, point);
			selectionPoints->SetPoint(j, point);
		}

		vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();
		for (int j = 0; j < dataset[i]->GetNumberOfCells(); j++)
		{
			vtkCell* cell = dataset[i]->GetCell(j);
			cellArray->InsertNextCell(cell);
		}

		vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
		polyData->SetPoints(selectionPoints);
		polyData->SetPolys(cellArray);

		double intersectPoints[3], pcoord[3], t;
		double projection[3];
		projection[0] = center[i][0];
		projection[1] = center[i][1];
		projection[2] = center[i][2];
		int sub;
		switch(view)
		{
		case FRONT3D:projection[2]+=100;break;
		case LEFT3D:projection[0]-=100;break;
		case RIGHT3D:projection[0]+=100;break;
		case TOP3D:projection[1]+=100;break;
		case BOTTOM3D:projection[1]-=100;break;
		case BACK3D:projection[2]-=100;break;
		}

		vtkSmartPointer<vtkCellLocator> cellLocator = 
		vtkSmartPointer<vtkCellLocator>::New();
		cellLocator->SetDataSet(polyData);
		cellLocator->BuildLocator();

		int result = cellLocator->IntersectWithLine(projection, center[i], 100, t, intersectPoints, pcoord, sub);

		if (result == 0)
		{
			double x = -1;
			visiblePoints.push_back(qMakePair(x, x));
		}
		else
		{
			vtkSmartPointer<vtkPoints> point = vtkSmartPointer<vtkPoints>::New();
			vtkSmartPointer<vtkPolyData> pointData = vtkSmartPointer<vtkPolyData>::New();
			point->SetNumberOfPoints(1);
			point->SetPoint(0, intersectPoints[0], intersectPoints[1], intersectPoints[2]);
			pointData->SetPoints(point);
			vtkSmartPointer<vtkSelectVisiblePoints> selectVisiblePoints = vtkSmartPointer<vtkSelectVisiblePoints>::New();
			selectVisiblePoints->SetInput(pointData);
			selectVisiblePoints->SetRenderer(gla->mRenderer);
			selectVisiblePoints->Update();
			if (selectVisiblePoints->GetOutput()->GetNumberOfPoints() != 0)
			{
				double *displayPt = new double[3];
				vtkInteractorObserver::ComputeWorldToDisplay (gla->mRenderer, intersectPoints[0], intersectPoints[1], intersectPoints[2], displayPt);
				visiblePoints.push_back(qMakePair(displayPt[0], displayPt[1]));
				delete displayPt;
			}
			else
			{
				double x = -1;
				visiblePoints.push_back(qMakePair(x, x));
			}
		}
	}
}

void ReportGenerator::detectCTSurfaceVisibility(vtkSmartPointer<vtkRenderer> render, 
		QVector<QPair<double*, CTSurfaceCornerPoint> > points, QVector<QPair<double*, CTSurfaceCornerPoint> >& visiblePoints)
{
	for (int i = 0; i < points.size(); i++)
	{
		vtkSmartPointer<vtkPoints> point = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkPolyData> pointData = vtkSmartPointer<vtkPolyData>::New();
		point->SetNumberOfPoints(1);
		point->SetPoint(0, points[i].first[0], points[i].first[1], points[i].first[2]);
		pointData->SetPoints(point);
		vtkSmartPointer<vtkSelectVisiblePoints> selectVisiblePoints = vtkSmartPointer<vtkSelectVisiblePoints>::New();
		selectVisiblePoints->SetInput(pointData);
		selectVisiblePoints->SetRenderer(render);
		selectVisiblePoints->Update();
		double *displayPt = new double[3];
		if (selectVisiblePoints->GetOutput()->GetNumberOfPoints() != 0)
		{
			vtkInteractorObserver::ComputeWorldToDisplay (render, points[i].first[0], points[i].first[1], points[i].first[2], displayPt);
			CTSurfaceCornerPoint corners;
			for (int j = 0; j < 4; j++)
			{
				vtkInteractorObserver::ComputeWorldToDisplay (render, points[i].second.point[j][0],
					points[i].second.point[j][1], points[i].second.point[j][2], corners.point[j]);
				qDebug()<<"in detect"<<corners.point[j][0]<<corners.point[j][1];
			}
			visiblePoints.push_back(qMakePair(displayPt, corners));
		}
		else
		{
			displayPt[0] = -1;
			displayPt[1] = -1;
			displayPt[2] = -1;
			visiblePoints.push_back(qMakePair(displayPt, points[i].second));
		}
	}
}

MainWindow* ReportGenerator::mw()
{
  foreach (QWidget *widget, QApplication::topLevelWidgets()) {
    MainWindow* mainwindow = dynamic_cast<MainWindow*>(widget);
    if (mainwindow)
    {
      return mainwindow;
    }
  }
  return NULL;
}

void ReportGenerator::saveWidgetinfo(const VtkWidget* gla, WidgetInfo3D &info)
{
	info.infoStatus = gla->getDisplayInfoOn();
	info.isMeasuring = gla->getUseRubberband();
	info.isAnnotation = gla->getUserAnnotationOn();
	vtkSmartPointer<vtkCamera> camera = gla->mRenderer->GetActiveCamera();
	camera->GetPosition(info.pos);
	camera->GetFocalPoint(info.foc);
	info.angle = camera->GetViewAngle();
	camera->GetViewUp(info.view);
	camera->GetClippingRange(info.clip);
	info.scale = camera->GetParallelScale();
	info.isDirectionalLight = gla->getIsDirectionalLight();

	vtkSmartPointer<vtkAssembly> assembly = mw()->mLightControl->GetAssembly();
	assembly->GetOrientation(info.orientation);

	info.brightness = mw()->mLightControl->GetIntensityL1();
	info.contrast = mw()->mLightControl->GetIntensityL2();
	info.renderMode = gla->getRenderMode3D();
	if (gla->getIsInterpolationOn()) 
		info.interpolation = 1;
	else if(!gla->getmRgbTextureFilename().isEmpty())
        info.interpolation = 0;
	else
		info.interpolation = -1;

	if(gla->getIsTextureOn()) 
		info.textureOn = 1;
	else if (!gla->getmRgbTextureFilename().isEmpty())
		info.textureOn = 0;
	else
		info.textureOn = -1;
}

void ReportGenerator::initWidget(VtkWidget* gla, bool isCTVolume)
{
	gla->setDisplayInfoOn(false);
	mw()->mLightControl->reset();
	if (gla->getIsTextureOn())
	{
		gla->setTextureOn(true);
	}
	if (isCTVolume)
	{
		gla->annotate(false);
	}
	else
	{
		gla->setRenderMode3D(SURFACE3D);
		gla->annotate(true);
	}
	gla->setVisibilityDistance(false);
	
}

void ReportGenerator::recoverWidget(VtkWidget* gla, WidgetInfo3D info, bool isCTVolume)
{
	gla->setDisplayInfoOn(info.infoStatus);
	vtkSmartPointer<vtkCamera> camera = gla->mRenderer->GetActiveCamera();
	camera->SetPosition(info.pos);
	camera->SetFocalPoint(info.foc);
	camera->SetViewAngle(info.angle);
	camera->SetViewUp(info.view);
	camera->SetClippingRange(info.clip);
	camera->SetParallelScale(info.scale);
	if(info.isDirectionalLight) 
		mw()->mLightControl->setDirectionalLight(true); 
	else 
		mw()->mLightControl->setDirectionalLight(false);

	if(info.textureOn == 1) 
		gla->setTextureOn(true);
	else if (info.textureOn == 0)
        gla->setTextureOn(false);

	if(info.interpolation == 1) 
		gla->setInterpolateOn(true);
	else if (info.interpolation == 0)
        gla->setInterpolateOn(false);

	mw()->mLightControl->restoreBookmarkLight(info.orientation, info.brightness, info.contrast, MODEL3D);
   
	if (!isCTVolume)	
		gla->setRenderMode3D(info.renderMode);
	gla->setMeasureDistance(info.isMeasuring);
	gla->annotate(info.isAnnotation);
	if(gla->mQVTKWidget) gla->mQVTKWidget->update();
}

void ReportGenerator::computeCenter(const VtkWidget* gla, QVector<int> cellIds, double* center)
{
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	for (int i = 0; i < cellIds.size(); i++)
	{
		vtkSmartPointer<vtkIdList> ptIds = vtkSmartPointer<vtkIdList>::New();
		gla->get3DPolyData()->GetCellPoints(cellIds[i], ptIds);
		double point[3];
		for (int j = 0; j < ptIds->GetNumberOfIds(); j++)
		{
			gla->get3DPolyData()->GetPoint(ptIds->GetId(j), point);
			points->InsertPoint(points->GetNumberOfPoints(), point);
		}
	}
	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(points);

	vtkSmartPointer<vtkCenterOfMass> centerOfMassFilter = vtkSmartPointer<vtkCenterOfMass>::New();
	#if VTK_MAJOR_VERSION <= 5
	centerOfMassFilter->SetInput(polyData);
	#else
	centerOfMassFilter->SetInputData(polyData);
	#endif
	centerOfMassFilter->SetUseScalarsAsWeights(false);
	centerOfMassFilter->Update();

	double CoM[3];
	centerOfMassFilter->GetCenter(CoM);
	vtkSmartPointer<vtkCellLocator> cellLocator = vtkSmartPointer<vtkCellLocator>::New();
	
	cellLocator->SetDataSet(gla->get3DPolyData());
	cellLocator->BuildLocator();
	double closestPointDist2;
	vtkIdType cellId;
	int subId;
	cellLocator->FindClosestPoint(CoM, center, cellId, subId, closestPointDist2);
}

void ReportGenerator::computeCenter(const VtkWidget* gla, vtkSmartPointer<vtkPoints> points, vtkSmartPointer<vtkDataArray> normals, 
									double* center, vtkSmartPointer<vtkDataSet>& polyData)
{
	vtkSmartPointer<vtkPoints> newPoints = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkDataArray> newNormals = normals;
	vtkSmartPointer<vtkPlanes> frustum = vtkSmartPointer<vtkPlanes>::New();
	newPoints->DeepCopy(points);
	newNormals->DeepCopy(normals);
	
	frustum->SetPoints(newPoints);
	frustum->SetNormals(newNormals);
	vtkSmartPointer<vtkExtractSelectedFrustum> extractor = vtkSmartPointer<vtkExtractSelectedFrustum>::New();
	extractor->SetInput(gla->get3DPolyData());
	extractor->PreserveTopologyOff();
	extractor->SetFrustum(frustum);
	extractor->Update();

	//vtkSmartPointer<vtkDataSet> polyData = vtkSmartPointer<vtkDataSet>::New();
	polyData  = vtkDataSet::SafeDownCast(extractor->GetOutput());
	
	vtkSmartPointer<vtkCenterOfMass> centerOfMassFilter = vtkSmartPointer<vtkCenterOfMass>::New();
	#if VTK_MAJOR_VERSION <= 5
	centerOfMassFilter->SetInput(polyData);
	#else
	centerOfMassFilter->SetInputData(polyData);
	#endif
	centerOfMassFilter->SetUseScalarsAsWeights(false);
	centerOfMassFilter->Update();

	double CoM[3];
	centerOfMassFilter->GetCenter(CoM);
	vtkSmartPointer<vtkCellLocator> cellLocator = vtkSmartPointer<vtkCellLocator>::New();
	
	cellLocator->SetDataSet(gla->get3DPolyData());
	cellLocator->BuildLocator();
	double closestPointDist2;
	vtkIdType cellId;
	int subId;
	cellLocator->FindClosestPoint(CoM, center, cellId, subId, closestPointDist2);
}

void ReportGenerator::computeCenter(CTSurfaceCornerPoint cornerPoints, double* center)
{
	center[0] = 0;
	center[1] = 0;
	center[2] = 0;
	for (int i = 0; i < 4; i++)
	{
		center[0] += cornerPoints.point[i][0]/4;
		center[1] += cornerPoints.point[i][1]/4;
		center[2] += cornerPoints.point[i][2]/4;
	}
}
