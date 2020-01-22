
if __name__ == "__main__":

    intervals = [1468948818,1468966818,1468984818,1469002818,1469020818,1469038818]
    averages = [0 for i in range(len(intervals)-1)]
    entries = [0 for i in range(len(intervals)-1)]
    
    filename = 'lumi_Fill5105_unixTime.dat'
    with open(filename) as f:
        lines = f.readlines()
        for l in lines:
            fields = l.split()
            if fields[3]!='STABLE_BEAMS': continue
            utc = int(fields[2])
            print fields
            if utc>intervals[-1]:
                print "Last UTC considered is ",utc,". Exiting now from the loop."
                break
            lumi = float(fields[5])/23000. #  luminosity delivered in 1 lumi-section 
            for i in range(len(averages)):
                if intervals[i]<utc<intervals[i+1]:
                    averages[i] += lumi
                    entries[i] += 1

    print "averages = ",averages
    print "entries = ",entries

    averages = [float(averages[i])/float(entries[i]) for i in range(len(averages))]

    print "normalized averages = ",averages
    
