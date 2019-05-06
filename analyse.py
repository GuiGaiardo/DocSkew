import os
import sys
import csv

def get_image_name(file_name):
    idx = file_name.index(".tif")

    return file_name[idx-4:idx]

def read_ground_truth(file_name):
    ground_truth = {}

    with open(file_name, 'r') as gt_file:
        for line in gt_file.readlines()[1:]:
            idx = line.index('SA')
            img_name = line[idx-5:idx-1]
            value = float(line[idx+3:])

            ground_truth[img_name] = value

    return ground_truth

def result_stats(ground_truth, my_results):
    error_distribution = []
    max_error = 0.1
    for img in my_results.keys():
        gt = ground_truth[img]
        mr = my_results[img]
        error = abs(gt-mr)
        if error > max_error:
            print("Error:{} Real:{} vs Found:{}  on {}".format(error, gt, mr, img))
        error_distribution.append(error)

    average_error = sum(error_distribution) / len(error_distribution)

    top80 = sorted(error_distribution)[:int(len(error_distribution) * 0.8)]
    average_top80 = sum(top80) / len(top80)

    bottom20 = sorted(error_distribution)
    bottom20 = bottom20[int(len(error_distribution) * 0.8):]

    average_bottom20 = sum(bottom20) / len(bottom20)

    correct_estimations = [x for x in error_distribution if x <= max_error]
    CE = len(correct_estimations) / len(error_distribution)

    print("Average error: {}".format(average_error))
    print("Average top80: {}".format(average_top80))
    print("CE: {}".format(CE))
    print("N Corrects: {}".format(len(correct_estimations)))
    print("Average bottom20: {}".format(average_bottom20))
    print("Max error: {}".format(max(bottom20)))


if len(sys.argv) < 3:
    print("Usage: python3 " + sys.argv[0] + " <results_file> <ground_truth_file>")
    exit()

result_file = sys.argv[1]
ground_truth = read_ground_truth(sys.argv[2])

my_results = {}
with open(result_file) as results:
    reader = csv.reader(results, delimiter=',')
    for line in reader:
        img_name = get_image_name(line[0])
        my_results[img_name] = -float(line[1])

result_stats(ground_truth, my_results)
