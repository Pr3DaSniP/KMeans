#include <iostream>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <random>
using namespace std;

class KMeans : public olc::PixelGameEngine
{
public:
	KMeans()
	{
		sAppName = "K-Moyenne";
	}

private:

	struct Point
	{
		double x;
		double y;
	};

	struct PointCluster
	{
		Point centroid;
		vector<Point> points;
		olc::Pixel color;
	};

	double distance(Point p1, Point p2)
	{
		return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
	}

	vector<PointCluster> generatePointCluster(int k, int xMin, int xMax, int yMin, int yMax)
	{
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> centroidGeneratorX(xMin, xMax);
		uniform_int_distribution<> centroidGeneratorY(yMin, yMax);
		uniform_int_distribution<> centroidGeneratorColor(0, 255);

		vector<PointCluster> pointClusters;
		for (int i = 0; i < k; i++)
		{
			PointCluster pointCluster;
			pointCluster.centroid.x = centroidGeneratorX(gen);
			pointCluster.centroid.y = centroidGeneratorY(gen);
			pointCluster.color = olc::Pixel(centroidGeneratorColor(gen), centroidGeneratorColor(gen), centroidGeneratorColor(gen));
			pointClusters.push_back(pointCluster);
		}
		return pointClusters;
	}

	vector<Point> loadPoints(const string& filename)
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

	double xMaxValue(const vector<Point>& pts)
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

	double xMinValue(const vector<Point>& pts)
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

	double yMaxValue(const vector<Point>& pts)
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

	double yMinValue(const vector<Point>& pts)
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

	bool critereStop(const vector<PointCluster>& pc, const vector<PointCluster>& prev_pc, double seuil)
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

	Point barycentre(const vector<Point>& pts)
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

	olc::Pixel getColorFromCluster(const Point& p, const vector<PointCluster>& pc)
	{
		for (auto& cluster : pc)
		{
			for (auto& point : cluster.points)
			{
				if (p.x == point.x && p.y == point.y)
				{
					return cluster.color;
				}
			}
		}
	}

	void render()
	{
		Clear(olc::BLACK);

		for (int i = 0; i < pts.size(); i++)
		{
			auto color = getColorFromCluster(pts[i], pointClusters);
			FillCircle(pts[i].x, pts[i].y, 3, color);
		}

		for (int i = 0; i < pointClusters.size(); i++)
		{
			FillCircle(pointClusters[i].centroid.x, pointClusters[i].centroid.y, 3, pointClusters[i].color);

			for (int j = 0; j < pointClusters[i].points.size(); j++)
			{
				DrawLine(
					pointClusters[i].centroid.x, pointClusters[i].centroid.y, 
					pointClusters[i].points[j].x, pointClusters[i].points[j].y, 
					pointClusters[i].color
				);
			}
		}
	}

	vector<Point> generateRandomPoints(int nb, int xMin, int xMax, int yMin, int yMax)
	{
		vector<Point> pts;
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> xGenerator(xMin, xMax);
		uniform_int_distribution<> yGenerator(yMin, yMax);
		for (int i = 0; i < nb; i++)
		{
			Point p;
			p.x = xGenerator(gen);
			p.y = yGenerator(gen);
			pts.push_back(p);
		}
		return pts;
	}

	vector<Point> pts;
	vector<PointCluster> pointClusters, prev_pointClusters;

protected:
	virtual bool OnUserCreate()
	{
		//pts = loadPoints("dataset.txt");
		pts = generateRandomPoints(500, 0, ScreenWidth(), 0, ScreenHeight());

		Point Max, Min;

		Max.x = xMaxValue(pts);
		Min.x = xMinValue(pts);
		Max.y = yMaxValue(pts);
		Min.y = yMinValue(pts);
	
		int nbCluster;
		std::cout << "Nombre de cluster : ";
		std::cin >> nbCluster;
		
		pointClusters = generatePointCluster(nbCluster, Min.x, Max.x, Min.y, Max.y);

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime)
	{
		this_thread::sleep_for(50ms);

		prev_pointClusters = pointClusters;

		for (int i = 0; i < pts.size(); i++)
		{
			double min = distance(pts[i], pointClusters[0].centroid);
			int index = 0;
			for (int j = 1; j < pointClusters.size(); j++)
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

		render();

		for (int i = 0; i < pointClusters.size(); i++)
		{
			pointClusters[i].centroid = barycentre(pointClusters[i].points);
			pointClusters[i].points.clear();
		}

		if (critereStop(pointClusters, prev_pointClusters, 0.1))
		{
			this_thread::sleep_for(5s);
			return false;
		}

		return true;
	}
};

int main() {
	
	KMeans kmeans;
	if (kmeans.Construct(1200, 700, 1, 1))
		kmeans.Start();

	return 0;
}