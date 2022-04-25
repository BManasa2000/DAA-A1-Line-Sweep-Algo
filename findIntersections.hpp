#ifndef FIND_INTERSECTIONS_HPP
#define FIND_INTERSECTIONS_HPP

#include "event.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

class FindIntersections
{
	private:
		/**
		* eventRoot - points to the root node of event AVL tree
		* statusRoot - points to the root node of status AVL tree
		* Q - an instance of eventTree class
		* J - an instance of statusTree class
		* intPts - will contain the intersection points along with the line segments
		* hor - vector containing horizontal line segments
		*/
		eventNode *eventRoot;
		statusNode *statusRoot;
		eventTree Q;
		statusTree J;
		vector<pair<point, vector<lineSegment> > > intPts; //will contain the final ans
        vector<lineSegment> hor;
	public:
		FindIntersections(vector<lineSegment> lines)
		{
			eventRoot = NULL;
			statusRoot = NULL;
			Q = eventTree();
			J = statusTree();

			int n = lines.size();
			for (int i = 0; i < n; i++)
			{
				double sx, sy, ex, ey;
				if (lines[i].sy == lines[i].ey)
                {
                    hor.push_back(lines[i]);
                    continue;
                }
				if (lines[i].sy > lines[i].ey || lines[i].sx < lines[i].ex)
				{
					sx = lines[i].sx;
					sy = lines[i].sy;
					ex = lines[i].ex;
					ey = lines[i].ey;
				}
				else if (lines[i].sy < lines[i].ey || lines[i].sx > lines[i].ex)
				{
					sx = lines[i].ex;
					sy = lines[i].ey;
					ex = lines[i].sx;
					ey = lines[i].sy;
				}

				lineSegment l;
				point sp;
				point ep;
				l = lines[i];
				lines[i].sx = sp.x = sx;
				lines[i].sy = sp.y = sy;
				lines[i].ex = ep.x = ex;
				lines[i].ey = ep.y = ey;
				eventRoot = Q.eventInsert(eventRoot, sp, l, 1);
				eventRoot = Q.eventInsert(eventRoot, ep, l, 2);
			}
			// Q.eventPreOrder(eventRoot);
			// J.statusPreOrder(statusRoot);

			// HANDLING HORIZONTAL LINES
            for(int i = 0; i < n; i++)
            {
                lineSegment l1 = lines[i];
                for (int j = 0; j < hor.size(); j++)
                {
                    if ((hor[j].sx == l1.sx) && (hor[j].sy == l1.sy) && (hor[j].ex == l1.ex) && (hor[j].ey == l1.ey))
                        continue;
                    double y_hor = hor[j].sy;
                    if ((y_hor <= l1.sy) && (y_hor >= l1.ey) && (l1.sy != l1.ey))
                    {
                        double x = J.findx(l1, y_hor);
                        if (hor[j].sx <= x && hor[j].ex >= x)
                        {
                        	pair<point, vector<lineSegment> > pt;
                        	pt.first.x = x;
                        	pt.first.y = y_hor;
                        	pt.second = vector<lineSegment> ();
                        	pt.second.push_back(hor[j]);
                        	pt.second.push_back(lines[i]);
                        	intPts.push_back(pt);
                        }
                    }
                }
            }
		}

		// #########################################################################################
		bool onSegment(point p, point q, point r)
		{
			if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
				return true;
			return false;
		}

		// #########################################################################################
		/**Function to find the nature of orientation of 3 points*/
		int orientation(point p, point q, point r)
		{
			double t = ((q.y - p.y) * (r.x - q.x)) - ((q.x - p.x) * (r.y - q.y));
			if (t == 0)
				return 0; // collinear points
			else if (t > 0)
				return 1; // clockwise
			return 2; // anti-clockwise
		}

		// #########################################################################################
		/**Function to check if 2 line segments intersect or not
		* @param l1 - 1st line segment 
		* @param l2 - 2nd line segment 
		* @retval true - if l1 and l2 intersect
		* @retval false - if l1 and l2 do not intersect
		*/
		bool doIntersect (lineSegment l1, lineSegment l2)
		{
			point p1, q1, p2, q2;

			// p1: upper end point of l1
			// q1: lower end point of l1
			// p2: upper end point of l2
			// q2: lower end point of l2
			p1.x = l1.sx;
			p1.y = l1.sy;
			q1.x = l1.ex;
			q1.y = l1.ey;

			p2.x = l2.sx;
			p2.y = l2.sy;
			q2.x = l2.ex;
			q2.y = l2.ey;

			int o1 = orientation(p1, q1, p2);
			int o2 = orientation(p1, q1, q2);
			int o3 = orientation(p2, q2, p1);
			int o4 = orientation(p2, q2, q1);

			// GENERAL CASE
			if (o1 != o2 && o3 != o4)
				return true;

			// SPECIAL CASES
			//if p1, q1 and p2 are collinear and p2 lies on p1q1
			if (o1 == 0 && onSegment (p1, p2, q1))
				return true;

			//if p1, q1 and q2 are collinear and q2 lies on p1q1
			if (o2 == 0 && onSegment(p1, q2, q1)) 
				return true; 
        
            // p2, q2 and p1 are colinear and p1 lies on p2q2 
            if (o3 == 0 && onSegment(p2, p1, q2)) 
            	return true; 
        
            // p2, q2 and q1 are colinear and q1 lies on p2q2 
            if (o4 == 0 && onSegment(p2, q1, q2)) 
            	return true; 
        
            return false;
		}

		// #########################################################################################
		/**Function that returns the intersection point of 2 line segments
		* @param l1 - 1st line segment 
		* @param l2 - 2nd line segment 
		* @retval (-1,-1) - if l1 and l2 do not intersect
		* @retval point of intersection - if l1 and l2 intersect
		*/
		point intersectionOf (lineSegment l1, lineSegment l2)
		{
			point ans;
			if (doIntersect(l1, l2) == 0) // if lines do not intersect return (-1, -1)
			{
				ans.x = -1;
				ans.y = -1;
			}
			else
			{
				// l1: a1x + b1y = c1
				double a1 = l1.ey - l1.sy;
				double b1 = l1.sx - l1.ex;
				double c1 = a1 * (l1.sx) + b1 * (l1.sy);

				// l2: a2x + b2y = c2
				double a2 = l2.ey - l2.sy;
				double b2 = l2.sx - l2.ex;
				double c2 = a2 * (l2.sx) + b2 * (l2.sy);

				double det = a1*b2 - a2*b1;
				ans.x = (b2*c1 - b1*c2) / det;
				ans.y = (a1*c2 - a2*c1) / det;
			}
			return ans;
		}
		
		// #########################################################################################
		/**Function to find new events*/
		void findNewEvent (lineSegment sl, lineSegment sr, eventNode *curEventPt)
		{
			point newEventPt = intersectionOf(sl, sr);
			if (newEventPt.y != -1)
			{
				if (newEventPt.y < curEventPt->p.y)
				{
					eventRoot = Q.eventInsert(eventRoot, newEventPt, sl, 3);
					eventRoot = Q.eventInsert(eventRoot, newEventPt, sr, 3);
				}
				else if (newEventPt.y == curEventPt->p.y && newEventPt.x > curEventPt->p.x)
				{
					eventRoot = Q.eventInsert(eventRoot, newEventPt, sl, 3);
					eventRoot = Q.eventInsert(eventRoot, newEventPt, sr, 3);
				}
			}
		}

		// #########################################################################################
		/**Function that checks if a line segment l is contained in vector v of line segments*/
		int contains (vector<lineSegment> v, lineSegment l)
		{
			for (int i = 0; i < v.size(); i++)
			{
				if (v[i].sx == l.sx && v[i].sy == l.sy && v[i].ex == l.ex && v[i].ey == l.ey)
					return 1;
			}
			return 0;
		}

		// #########################################################################################
		/**Function that returns the union of 2 vectors of line segments*/
		vector<lineSegment> unionOf(vector<lineSegment> a, vector<lineSegment> b)
		{
			vector<lineSegment> unionVec;
			for (int i = 0; i < a.size(); i++)
			{
				unionVec.push_back(a[i]);
			}
			for (int i = 0; i < b.size(); i++)
			{
				if (contains(unionVec, b[i]) == 0) // i.e unionVec doesn't contain b[i]
				{
					unionVec.push_back(b[i]);
				}
			}
			return unionVec;
		}

		// #########################################################################################
		/**Function that checks if a vector of line segments is empty or not
		* @param v - vector of line segments
		* @retval 1 if v is empty
		* @retval 0 if v is not empty
		*/
		int empty (vector<lineSegment> v)
		{
			if (v.size() == 0)
				return 1;
			return 0;
		}

		// #########################################################################################
		/**Function to handle event points
		* @param curEventPt - current event point 
		*/
		void handleEventPoint (eventNode *curEventPt)
		{
			vector<lineSegment> all = unionOf(curEventPt->L, unionOf(curEventPt->U, curEventPt->C));

			if (all.size() > 1)
			{
				pair<point, vector<lineSegment> > pt;
				pt.first = curEventPt->p;
				pt.second = vector<lineSegment> ();
				for (int i = 0; i < all.size(); i++)
					pt.second.push_back(all[i]);
				intPts.push_back(pt);
			}

			//remove segments in L(p) U C(p) from J
			vector<lineSegment> delLines = unionOf(curEventPt->L, curEventPt->C);
			// for (auto l: delLines)
			// cout << "Size of delLines: " << delLines.size() << endl;
			for (int i = 0; i < delLines.size(); ++i)
			{
				// printf("(%f, %f), (%f, %f)\n", delLines[i].sx, delLines[i].sy, delLines[i].ex, delLines[i].ey);
				statusRoot = J.statusDelete(statusRoot, delLines[i], curEventPt->p.y);
			}

			//insert segments in U(p) U C(p) in J
			vector<lineSegment> insLines = unionOf(curEventPt->U, curEventPt->C);

			// for (auto l: insLines)
			for (int i = 0; i < insLines.size(); ++i)
			{
				// printf("(%f, %f), (%f, %f)\n", insLines[i].sx, insLines[i].sy, insLines[i].ex, insLines[i].ey);
				statusRoot = J.statusInsert(statusRoot, insLines[i], curEventPt->p.y);
			}

			if (empty(insLines) == 1)
			{
				lineSegment sl, sr;
				sl.sx = -1;
				sr.sx = -1;
				J.getNeighbors(statusRoot, curEventPt->p.x, (curEventPt->p.y) - 0.1, &sl, &sr);
			}
			else
			{
				lineSegment sll, srr;
				double max = -1.0, min = 1001.0;
				// for (auto l: insLines)
				for (int i = 0; i < insLines.size(); i++)
				{
					lineSegment l = insLines[i];
					double x = J.findx(l, curEventPt->p.y);
					double minX = (l.sx < l.ex) ? l.sx: l.ex;
					double maxX = (l.sx > l.ex) ? l.sx: l.ex;
					if (x < min && x <= maxX && x >= minX)
					{
						min = x;
						sll = l;
					}
					if (x > max && x <= maxX && x >= minX)
					{
						max = x;
						srr = l;
					}
				}

				lineSegment sl, sr;
				sl.sx = -1;
				sr.sx = -1;
				J.getLeftNeighbor(statusRoot, sll, curEventPt->p.y, &sl);					
				J.getRightNeighbor(statusRoot, srr, curEventPt->p.y, &sr);					

				if (sl.sx != -1 && min != 1001)
					findNewEvent(sl, sll, curEventPt);

				if (sr.sx != -1 && max != -1)
					findNewEvent(srr, sr, curEventPt);
			}
		}

		// #########################################################################################
		/**Driver code for finding intersections*/
		vector<point> run()
		{
			while (eventRoot != NULL)
			{
				eventNode* pop = Q.eventMaxValueNode (eventRoot);
				if (pop != NULL)
				{
					// cout << "Popped event: " << pop->p.x << ", " << pop->p.y << endl;					
					handleEventPoint(pop);
					eventRoot = Q.eventDelete(eventRoot, pop->p);
				}
				/*cout << "\nPrinting STATUS queue!\n";
				J.statusPreOrder(statusRoot);
				cout << endl;*/

				/*cout << "Printing EVENT queue!\n";
				Q.eventPreOrder(eventRoot);
				cout << endl << endl;*/
				// Q.eventPreOrder(eventRoot);

			}

			vector<point> ans;
    		fstream file;
    		file.open("intersection_pts.txt", ios::out); // input file which contains the intersection points
    		if (file.is_open())
    		{
    			for (int i = 0; i < intPts.size(); i++)
    				file << intPts[i].first.x << " " << intPts[i].first.y << endl;
    		}
        	file.close(); //close the file object
			cout << "Number of intersection points: " << intPts.size() << endl << endl;
			if (intPts.size() != 0)
			{
				for (int i = 0; i < intPts.size(); i++)
				{
					ans.push_back(intPts[i].first);
					cout << "Intersection Point: " << intPts[i].first.x << " " << intPts[i].first.y << endl;
					for (int j = 0; j < intPts[i].second.size(); j++)
					{
						printf("Line #%d: (%f, %f), (%f, %f)\n", j+1, intPts[i].second[j].sx, intPts[i].second[j].sy, intPts[i].second[j].ex, intPts[i].second[j].ey);
					}
					cout << endl;
				}
			} 
			return ans;
		}

		// #########################################################################################
};


#endif








