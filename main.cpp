#include "findIntersections.hpp"
#include <fstream>

int main()
{
    /*fstream file1, file2, file3, file4;
    file1.open("log.txt", ios::in); // input file which contains 10 test cases
    file2.open("output.txt",ios::out); // will store the #intersection points followed by the intersection points
    file3.open("runtime_int.txt",ios::out); // will store #intersection pts followed by runtime
    file4.open("runtime_lin.txt",ios::out); // will store #line segments followed by runtime
    int t = 20;
    if (file1.is_open() && file2.is_open() && file3.is_open() && file4.is_open())
    {
        // file1 >> t;
        // 10 testcases in input file
        for (int i = 0; i < t; i++)
        {
            int n;
            file1 >> n;
            // cout << n << endl;
            vector<lineSegment> lines;
            for (int j = 0; j < n; j++)
            {
                lineSegment l;
                file1 >> l.sx >> l.sy >> l.ex >> l.ey;
                lines.push_back(l);
            }

            // cout << endl << "OUTPUT #" << i+1 << endl;

            vector<point> v;
            clock_t start, end;
            start = clock();
            ios_base::sync_with_stdio(false);
            FindIntersections findInt = FindIntersections(lines);
            v = findInt.run();

            file2 << v.size() << endl;
            for (int i = 0; i < v.size(); i++)
            {
                file2 << v[i].x << " " << v[i].y << endl;
            }
            end = clock();
            double time = double(end - start)/double(CLOCKS_PER_SEC);
            file3 << v.size() << " " << time << endl;
            file4 << n << " " << time << endl;
            // cout<<"Time taken is: " << fixed << time << setprecision(10) << endl;
            // cout << "DONE WITH TEST CASE #" << i+1 << endl;
        }
        file1.close(); //close the input file object
        file2.close(); //close the output file object
        file3.close(); //close the runtime file object
    }*/

    // code if input is to be taken manually from user
	int n;
	cout << "Pls enter the no. of line segments: ";
	cin >> n;
	vector<lineSegment> lines;
    cout << "Pls enter the end points: \n";
	for (int i = 0; i < n; i++)
	{
		cout << "\nPls enter the start point coordinates (sx, sy):\n";
		lineSegment l;
		cin >> l.sx >> l.sy;
		cout << "Pls enter the end point coordinates (ex, ey):\n";
		cin >> l.ex >> l.ey;
		lines.push_back(l);
	}

	FindIntersections findInt = FindIntersections(lines);
	findInt.run();
}



















