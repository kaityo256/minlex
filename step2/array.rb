class Array
	include Comparable
	def to_s
		self.join("")
	end
end

class String
	def to_a
		self.each_char.map{|c| c.ord - 48}
	end
end

$min = [9]*81

def renumbering(s)
	a = Array.new(9){0}
	index = 0
	b = s.dup
	81.times do |i|
		next if s[i] == 0
		c = s[i]-1
		if a[c] == 0
			index = index + 1
			a[c] = index
		end
		b[i] = a[c]
	end
	b
end

def transpose(s)
	s2 = Array.new(81){0}
	9.times do |i|
		9.times do |j|
			s2[j + i*9] = s[i + j*9]
		end
	end
	s2
end

def headline_index(s)
	sa = s[0..8].each_slice(3).to_a
	a2 = sa.map do |ai|
		ai.inject(0) {|sum,v| v!=0 ? sum + 1: sum}
	end
	a2
end

def headbox_index(s)
	sh = s[0..26].each_slice(9).to_a
	sh.map do |shi|
		sa = shi.each_slice(3).to_a
		a2 = sa.map do |ai|
			ai.inject(0) {|sum,v| v!=0 ? sum + 1: sum}
		end
		a2
	end.min
end

def headbox_sorted_index(s)
	sh = s[0..26].each_slice(9).to_a
	sh.map do |shi|
		sa = shi.each_slice(3).to_a
		a2 = sa.map do |ai|
			ai.inject(0) {|sum,v| v!=0 ? sum + 1: sum}
		end.sort
		a2
	end.min
end

def perm_restrbox(s)
	sh,sm,sb = s.each_slice(27).to_a
	sma = sm.each_slice(9).to_a
	sba = sb.each_slice(9).to_a
	[0,1,2].permutation do |ai|
		si = sma[ai[0]] + sma[ai[1]] + sma[ai[2]]
		[0,1,2].permutation do |aj|
			sj = sba[aj[0]] + sba[aj[1]] + sba[aj[2]]
			ss = renumbering(sh + si + sj)
			if ss < $min
				$min = ss
			end
		end
	end
end

def perm_columns(s)
	st = transpose(s)
	sa = st.each_slice(9).to_a
	[0,1,2].permutation do |ai|
		si = sa[ai[0]] + sa[ai[1]] + sa[ai[2]]
		[3,4,5].permutation do |aj|
			sj = sa[aj[0]] + sa[aj[1]] + sa[aj[2]]
			[6,7,8].permutation do |ak|
				sk = sa[ak[0]] + sa[ak[1]] + sa[ak[2]]
				ss = transpose(si+sj+sk)
				ss = renumbering(ss)
				next if ss.slice(0,27) > $min.slice(0,27)
				perm_restrbox(ss)
			end
		end
	end
end

def perm_toprbox(s,hb_min)
	sh,sm,sb = s.each_slice(27).to_a
	sa = sh.each_slice(9).to_a
	[0,1,2].permutation do |ai|
		si = sa[ai[0]] + sa[ai[1]] + sa[ai[2]]
		si = si + sm + sb
		next if headline_index(si) > hb_min
		perm_columns(si)
	end
end

def perm_cbox(s,hb_min)
	st = transpose(s)
	sa = st.each_slice(27).to_a
	[0,1,2].permutation do |ai|
		si = sa[ai[0]] + sa[ai[1]] + sa[ai[2]]
		si = transpose(si)
		next if headbox_index(si) > hb_min
		perm_toprbox(si,hb_min)
	end
end

def perm_rbox(s)
	sa = s.each_slice(27).to_a
	[0,1,2].permutation do |ai|
		si = sa[ai[0]] + sa[ai[1]] + sa[ai[2]]
		perm_cbox(si)
	end
end

def search(s)
	a = []
	sa = s.each_slice(27).to_a
	[0,1,2].permutation do |ai|
		a.push sa[ai[0]] + sa[ai[1]] + sa[ai[2]]
	end
	sa = transpose(s).each_slice(27).to_a
	[0,1,2].permutation do |ai|
		a.push sa[ai[0]] + sa[ai[1]] + sa[ai[2]]
	end
	hb_min = a.map{|v| headbox_sorted_index(v)}.min
	a.each do |si|
		next if headbox_sorted_index(si) > hb_min
		perm_cbox(si,hb_min)
	end
end

s = "207005000000340000150000009005000001040000320000016500000002084700000010010580000".to_a
ans = "000000012000034005006007300001300007053080000080000100010005090200100000700400030".to_a
puts s.to_s
search(s)
puts $min.to_s
if $min == ans
	puts "OK"
else
	puts "NG"
end
