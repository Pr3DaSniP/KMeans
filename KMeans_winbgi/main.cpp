#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <time.h>

#include "graphics.h"

using namespace std;

void createDataset(int nbr, int xMin, int xMax, int yMin, int yMax)
{
	ofstream file("dataset.txt", ios::app);
	if (file.is_open())
	{
		for (int i = 0; i < nbr; i++)
		{
			double x = (double)rand() / RAND_MAX * (xMax - xMin) + xMin;
			double y = (double)rand() / RAND_MAX * (yMax - yMin) + yMin;
			file << x << " " << y << endl;
		}
		file.close();
	}
}

struct Point
{
	double x;
	double y;
};

struct PointCluster
{
	Point centroid;
	vector<Point> points;
};

double distance(Point p1, Point p2)
{
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

vector<PointCluster> generatePointCluster(int k, int xMin, int xMax, int yMin, int yMax)
{
	vector<PointCluster> pointClusters;
	for (int i = 0; i < k; i++)
	{
		PointCluster pointCluster;
		pointCluster.centroid.x = rand() % (xMax - xMin) + xMin;
		pointCluster.centroid.y = rand() % (yMax - yMin) + yMin;
		pointClusters.push_back(pointCluster);
	}
	return pointClusters;
}

vector<Point> loadPoints(const string &filename)
{
	vector<Point> points;
	ifstream file(filename);
	if (file.is_open())
	{
		string line;
		while (getline(file, line))
		{
			Point point;
			stringstream ss(line);
			ss >> point.x >> point.y;
			points.push_back(point);
		}
		file.close();
	}
	return points;
}

double xMaxValue(const vector<Point> &pts)
{
	double max = pts[0].x;
	for (int i = 1; i < pts.size(); i++)
	{
		if (pts[i].x > max)
		{
			max = pts[i].x;
		}
	}
	return max;
}

double xMinValue(const vector<Point> &pts)
{
	double min = pts[0].x;
	for (int i = 1; i < pts.size(); i++)
	{
		if (pts[i].x < min)
		{
			min = pts[i].x;
		}
	}
	return min;
}

double yMaxValue(const vector<Point> &pts)
{
	double max = pts[0].y;
	for (int i = 1; i < pts.size(); i++)
	{
		if (pts[i].y > max)
		{
			max = pts[i].y;
		}
	}
	return max;
}

double yMinValue(const vector<Point> &pts)
{
	double min = pts[0].y;
	for (int i = 1; i < pts.size(); i++)
	{
		if (pts[i].y < min)
		{
			min = pts[i].y;
		}
	}
	return min;
}

bool critereStop(const vector<PointCluster> &pc, const vector<PointCluster> &prev_pc, double seuil)
{
	for (int i = 0; i < pc.size(); i++)
	{
		if (distance(pc[i].centroid, prev_pc[i].centroid) > seuil)
		{
			return false;
		}
	}
	return true;
}

Point barycentre(const vector<Point> &pts)
{
	Point barycentre;
	double x = 0;
	double y = 0;
	for (int i = 0; i < pts.size(); i++)
	{
		x += pts[i].x;
		y += pts[i].y;
	}
	barycentre.x = x / pts.size();
	barycentre.y = y / pts.size();
	return barycentre;
}

void sleep(int milliseconds)
{
	clock_t time_end;
	time_end = clock() + milliseconds * CLOCKS_PER_SEC / 1000;
	while (clock() < time_end)
	{
	}
}

void trace(const vector<Point> &pts, const vector<PointCluster> &pc)
{
	setcolor(WHITE);
	for (int i = 0; i < pts.size(); i++)
	{
		rectangle(pts[i].x, pts[i].y, pts[i].x + 5, pts[i].y + 5);
	}

	for (int i = 0; i < pc.size(); i++)
	{
		setcolor(i + 1);
		rectangle(pc[i].centroid.x, pc[i].centroid.y, pc[i].centroid.x + 5, pc[i].centroid.y + 5);

		for (int j = 0; j < pc[i].points.size(); j++)
		{
			line(pc[i].centroid.x, pc[i].centroid.y, pc[i].points[j].x, pc[i].points[j].y);
		}
	}
}

int main()
{
	srand(time(NULL));
	clock_t t1, t2;

	auto pts = loadPoints("dataset.txt");

	double xMax = xMaxValue(pts);
	double xMin = xMinValue(pts);
	double yMax = yMaxValue(pts);
	double yMin = yMinValue(pts);

	cout << "K = ";
	int k;
	cin >> k;

	vector<PointCluster> pointClusters = generatePointCluster(k, xMin, xMax, yMin, yMax);
	vector<PointCluster> prev_pointClusters;

	opengraphsize(1200, 600);

	t1 = clock();
	int iter = 0;
	do
	{
		cout << iter++ << endl;
		for (int i = 0; i < pts.size(); ++i)
		{
			double min = distance(pts[i], pointClusters[0].centroid);
			int index = 0;
			for (int j = 1; j < pointClusters.size(); ++j)
			{
				double dist = distance(pts[i], pointClusters[j].centroid);
				if (dist < min)
				{
					min = dist;
					index = j;
				}
			}
			pointClusters[index].points.push_back(pts[i]);
		}

		cleardevice();
		trace(pts, pointClusters);
		sleep(250);

		prev_pointClusters = pointClusters;
		for (int i = 0; i < pointClusters.size(); ++i)
		{
			pointClusters[i].centroid = barycentre(pointClusters[i].points);
			pointClusters[i].points.clear();
		}
	} while (!critereStop(pointClusters, prev_pointClusters, 0.1));

	for (int i = 0; i < pts.size(); ++i)
	{
		double min = distance(pts[i], pointClusters[0].centroid);
		int index = 0;
		for (int j = 1; j < pointClusters.size(); ++j)
		{
			double dist = distance(pts[i], pointClusters[j].centroid);
			if (dist < min)
			{
				min = dist;
				index = j;
			}
		}
		pointClusters[index].points.push_back(pts[i]);
	}
	t2 = clock();

	cleardevice();
	trace(pts, pointClusters);

	for (int i = 0; i < pointClusters.size(); ++i)
	{
		cout << "Cluster " << i + 1 << endl;
		cout << "Centroid: " << pointClusters[i].centroid.x << " " << pointClusters[i].centroid.y << endl;
		cout << endl;
	}

	cout << "Temps d'execution: " << (double)(t2 - t1) / CLOCKS_PER_SEC << " secondes" << endl;

	getch();

	closegraph();

	return 0;
}
