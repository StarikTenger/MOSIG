/*
 * Creative commons CC BY 2023 Nadia Brauner <nadia.brauner@grenoble-inp.fr>
 *
 * See <https://creativecommons.org/licenses/by/4.0/>.
 */
package org.caseine.or.dynamic_prog;

import java.util.*;

// dynamic programming: integer Knapsack
public class IntegerKnapsack {

    /**
     * Given three arrays of integers of same size, utility, weight and quantity of each type of object
     * and an integer, the capacity of the knapsack,
     * choose for each type of object, the number of objects of this type in the knapasack
     * such that this number is smaller than the quantity,
     * the sum of the utilities of the chosen objects is maximal
     * and the sum of their weights is smaller than the capacity
     *
     * Your method should return the value of the knapsack,
     * i.e. the sum of the utilities of the chosen object
     *
     * @param utilities: an array of integer values, the utility of each type of objects
     * @param weights: an array of integer values, the weight of each type of objects
     * @param quantities: an array of integer values, the quantity of each type of objects
     * @param capacity: bounds the weights of the objects in the knapsack
     *
     * @return an integer which is the value of the optimum knapsack in the knapsack
     */
    public static int integerKnapsack(int[] utilities, int[] weights, int[] quantities, int capacity) {
        return 0;
    // TODO
    }

    public static void main(String[] args) {
        int result = integerKnapsack(new int[] { 1, 3, 1, 1, 1, 1 }, new int[] { 8, 12, 5, 3, 6, 9 }, new int[] { 4, 4, 4, 4, 4, 4 }, 33);
        System.out.print(result);
    }
}
