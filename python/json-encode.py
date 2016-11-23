import json

class output_format :
  start_time = ""
  end_time = ""
  skew = ""
  def __init__(self, st, et, skew):
    self.start_time = st
    self.end_time = et
    self.skew = skew

def op_default(obj):
  return obj.__dict__
  
of1 = output_format(10, 20, 99)
of2 = output_format(40, 50, 98)

list_of_output_formats = []
list_of_output_formats.append(of1)
list_of_output_formats.append(of2)

for of in list_of_output_formats:
  print json.dumps({"start_time" : of.start_time, "end_time " : of.end_time, "skew " : of.skew } , default=op_default, indent = 4)