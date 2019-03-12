class Integer
  def to_b
    sprintf("%03b",self)
  end
end

flag = [true,true,false, true,false,false,false,true]

ai = []
8.times do |i|
  aj = []
  [0,1,2].permutation do |a|
    t = []
    t.push (i&4) >> 2
    t.push (i&2) >> 1
    t.push (i&1)
    j = 0
    j = j + t[0] * (1 << (2-a.index(0)))
    j = j + t[1] * (1 << (2-a.index(1)))
    j = j + t[2] * (1 << (2-a.index(2)))
    puts "#{t} #{i.to_b} #{j.to_b} #{a} #{flag[j]}"
    aj.push flag[j]
  end
  ai.push "{" + aj.join(",") + "}"
end
puts "{" + ai.join(",") + "}"
